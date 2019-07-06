#pragma once

#include "control/common/Widget.hpp"
#include "control/common/IOverlayLayout.hpp"
#include "IMainWindow.hpp"

#include <gtkmm/window.h>

using SignalKeyPressed = sigc::signal<void(std::string)>;
using SignalWindowRealized = sigc::signal<void()>;

class MainWindow : public Widget<IMainWindow>
{
public:
    MainWindow();

    void showAll() override;
    void scale(double scaleX, double scaleY) override;
    void setSize(int width, int height) override;
    int width() const override;
    int height() const override;
    int x() const override;
    int y() const override;

    void setMainLayout(const std::shared_ptr<IWidget>& child) override;
    void setOverlays(const std::vector<std::shared_ptr<IOverlayLayout>>& children) override;
    void move(int x, int y) override;
    void disableWindowResize() override;
    void disableWindowDecoration() override;
    void setKeepAbove(bool keepAbove) override;
    void fullscreen() override;
    void unfullscreen() override;
    void setCursorVisible(bool cursorVisible) override;

    SignalKeyPressed keyPressed() override;
    Gtk::Window& get() override;

private:
    void setWindowSize(int width, int height);
    void onRealized();
    void loadDefaultStyle();
    Gdk::Rectangle getCurrentMonitorGeometry() const;

private:
    Gtk::Window m_handler;
    std::shared_ptr<IOverlayLayout> m_layout;
    SignalWindowRealized m_resizeSignal;
    SignalKeyPressed m_keyPressed;
    sigc::connection m_windowState;
    int m_originalWidth, m_originalHeight;
    int m_xPos = -1, m_yPos = -1;
    bool m_cursorVisible = true;

};
