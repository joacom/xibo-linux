#include "MainWindowController.hpp"

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/messagedialog.h>

#include <boost/process/child.hpp>

#include "networking/xmds/XmdsRequestSender.hpp"
#include "config.hpp"
#include "HardwareKey.hpp"

MainWindowController::MainWindowController(Gtk::Window* window, const Glib::RefPtr<Gtk::Builder>& ui) :
    m_ui(ui),
    m_mainWindow(window)
{
    m_settings.loadFrom(ProjectResources::cmsSettingsFile());

    initUi();
    updateControls(m_settings);
    connectSignals();
}

void MainWindowController::initUi()
{
    m_ui->get_widget(Resources::Ui::CmsAddressEntry, m_cmsAddressField);
    m_ui->get_widget(Resources::Ui::KeyEntry, m_keyField);
    m_ui->get_widget(Resources::Ui::ResourcesPathEntry, m_resourcesPathField);
    m_ui->get_widget(Resources::Ui::BrowseResourcesButton, m_browseResourcesPath);

    m_ui->get_widget(Resources::Ui::UsernameEntry, m_usernameField);
    m_ui->get_widget(Resources::Ui::PasswordEntry, m_passwordField);
    m_ui->get_widget(Resources::Ui::SplashScreenPathEntry, m_splashScreenPath);
    m_ui->get_widget(Resources::Ui::BrowseSplashScreenButton, m_browseSplashScreenPath);
    m_ui->get_widget(Resources::Ui::DomainEntry, m_domainField);
    m_ui->get_widget(Resources::Ui::DisplayIdEntry, m_displayIdField);

    m_ui->get_widget(Resources::Ui::ConnectionStatusLabel, m_connectionStatus);
    m_ui->get_widget(Resources::Ui::SaveButton, m_saveSettings);
    m_ui->get_widget(Resources::Ui::LaunchClientButton, m_launchClient);
    m_ui->get_widget(Resources::Ui::DisplayAdminButton, m_displayAdmin);
    m_ui->get_widget(Resources::Ui::ExitButton, m_exit);
}

void MainWindowController::updateControls(const CmsSettings& settings)
{
    m_cmsAddressField->set_text(Glib::ustring{settings.cmsAddress()});
    m_keyField->set_text(Glib::ustring{settings.key()});
    m_resourcesPathField->set_text(Glib::ustring{settings.resourcesPath()});

    m_usernameField->set_text(Glib::ustring{settings.username()});
    m_passwordField->set_text(Glib::ustring{settings.password()});
    m_domainField->set_text(Glib::ustring{settings.domain()});
    m_displayIdField->set_text(Glib::ustring{settings.displayId()});
}

void MainWindowController::connectSignals()
{
    m_saveSettings->signal_clicked().connect(sigc::mem_fun(this, &MainWindowController::onSaveSettingsClicked));
    m_launchClient->signal_clicked().connect(sigc::mem_fun(this, &MainWindowController::onLaunchClientClicked));
    m_displayAdmin->signal_clicked().connect(sigc::mem_fun(this, &MainWindowController::onDisplayAdminClicked));
    m_browseResourcesPath->signal_clicked().connect(sigc::mem_fun(this, &MainWindowController::onBrowseResourcesPathClicked));
    m_exit->signal_clicked().connect(sigc::mem_fun(m_mainWindow, &Gtk::Window::close));
}

void MainWindowController::onSaveSettingsClicked()
{
    auto displayId = getDisplayId();
    auto connectionResult = connectToCms(m_cmsAddressField->get_text(), m_keyField->get_text(), displayId);

    m_connectionStatus->set_text(connectionResult);
    m_displayIdField->set_text(displayId);

    updateSettings();
}

std::string MainWindowController::getDisplayId()
{
    std::string displayId = m_displayIdField->get_text();

    return displayId.empty() ? HardwareKey::generate() : displayId;
}

std::string MainWindowController::connectToCms(const std::string& cmsAddress, const std::string& key, const std::string& displayId)
{
    try
    {
        XmdsRequestSender xmdsRequester{cmsAddress, key, displayId};

        auto connectionResult = xmdsRequester.registerDisplay(ProjectResources::codeVersion(),
                                                              ProjectResources::version(),
                                                              "Display").then([](auto future){
            auto [error, result] = future.get();

            if(!error)
                return result.status.message;
            else
                return error.message();
        });

        return connectionResult.get();
    }
    catch(std::exception& e)
    {
        return e.what();
    }
}

void MainWindowController::updateSettings()
{
    CmsSettings settings;

    settings.setCmsAddress(m_cmsAddressField->get_text());
    settings.setKey(m_keyField->get_text());
    std::string path = m_resourcesPathField->get_text();
    settings.setResourcesPath(path.empty() ? ProjectResources::defaultResourcesDir().string() : path);

    settings.setUsername(m_usernameField->get_text());
    settings.setPassword(m_passwordField->get_text());
    settings.setDomain(m_domainField->get_text());
    settings.setDisplayId(m_displayIdField->get_text());

    m_settings.saveTo(ProjectResources::cmsSettingsFile());
}

void MainWindowController::onLaunchClientClicked()
{
    boost::process::child player{ProjectResources::playerBinary()};

    m_mainWindow->close();

    player.wait();
}

void MainWindowController::onBrowseResourcesPathClicked()
{
    Gtk::FileChooserDialog dialog{Resources::ChooseResourcesFolder, Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER};

    dialog.set_transient_for(*m_mainWindow);
    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Select", Gtk::RESPONSE_OK);

    int result = dialog.run();

    switch(result)
    {
        case(Gtk::RESPONSE_OK):
            m_resourcesPathField->set_text(dialog.get_filename());
            break;
        default:
            break;
    }
}

void MainWindowController::onDisplayAdminClicked()
{
//    m_mainWindow->show_uri(m_cmsAddress->get_text(), static_cast<unsigned int>(std::time(nullptr)));
}
