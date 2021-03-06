//----------------------------------------------------------------------------
// Cef3D
// Copyright (C) 2017 arkenthera
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// https://github.com/arkenthera/cef3d
// Cef3D.h
// Date: 13.04.2017
//---------------------------------------------------------------------------

#pragma once

#include "include/cef_menu_model_delegate.h"
#include "include/views/cef_button_delegate.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_browser_view_delegate.h"
#include "include/views/cef_label_button.h"
#include "include/views/cef_menu_button.h"
#include "include/views/cef_menu_button_delegate.h"
#include "include/views/cef_textfield.h"
#include "include/views/cef_textfield_delegate.h"
#include "include/views/cef_window.h"
#include "include/views/cef_window_delegate.h"

namespace Cef3D
{

	/* */

	class CEF3D_API ViewsWindow : public CefBrowserViewDelegate,
		public CefMenuButtonDelegate,
		public CefMenuModelDelegate,
		public CefTextfieldDelegate,
		public CefWindowDelegate 
	{
	public:
		// Delegate methods will be called on the browser process UI thread.
		class Delegate {
		public:
			// Return the initial window bounds.
			virtual CefRect GetWindowBounds() = 0;

			// Called when the ViewsWindow is created.
			virtual void OnViewsWindowCreated(CefRefPtr<ViewsWindow> window) = 0;

			// Called when the ViewsWindow is destroyed. All references to |window|
			// should be released in this callback.
			virtual void OnViewsWindowDestroyed(CefRefPtr<ViewsWindow> window) = 0;

			// Return the Delegate for the popup window controlled by |client|.
			virtual Delegate* GetDelegateForPopup(CefRefPtr<CefClient> client) = 0;

			// Called to exit the application.
			virtual void OnExit() = 0;

		protected:
			virtual ~Delegate() {}
		};

		// Create a new top-level ViewsWindow hosting a browser with the specified
		// configuration.
		static CefRefPtr<ViewsWindow> Create(
			Delegate* delegate,
			CefRefPtr<CefClient> client,
			const CefString& url,
			const CefBrowserSettings& settings,
			CefRefPtr<CefRequestContext> request_context);

		void Show();
		void Hide();
		void Minimize();
		void Maximize();
		void SetBounds(const CefRect& bounds);
		void Close(bool force);
		void SetAddress(const std::string& url);
		void SetTitle(const std::string& title);
		void SetFavicon(CefRefPtr<CefImage> image);
		void SetFullscreen(bool fullscreen);
		void SetLoadingState(bool isLoading,
			bool canGoBack,
			bool canGoForward);
		void SetDraggableRegions(const std::vector<CefDraggableRegion>& regions);

		int GetBrowserIdentifier();

		// CefBrowserViewDelegate methods:
		bool OnPopupBrowserViewCreated(
			CefRefPtr<CefBrowserView> browser_view,
			CefRefPtr<CefBrowserView> popup_browser_view,
			bool is_devtools) OVERRIDE;

		// CefButtonDelegate methods:
		void OnButtonPressed(CefRefPtr<CefButton> button) OVERRIDE;

		// CefMenuButtonDelegate methods:
		void OnMenuButtonPressed(CefRefPtr<CefMenuButton> menu_button,
			const CefPoint& screen_point) OVERRIDE;

		// CefMenuModelDelegate methods:
		void ExecuteCommand(CefRefPtr<CefMenuModel> menu_model,
			int command_id,
			cef_event_flags_t event_flags) OVERRIDE;

		// CefTextfieldDelegate methods:
		bool OnKeyEvent(CefRefPtr<CefTextfield> textfield,
			const CefKeyEvent& event) OVERRIDE;

		// CefWindowDelegate methods:
		void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE;
		void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE;
		bool IsFrameless(CefRefPtr<CefWindow> window) OVERRIDE;
		bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE;

		// CefViewDelegate methods:
		CefSize GetMinimumSize(CefRefPtr<CefView> view) override;

	private:
		// |delegate| is guaranteed to outlive this object.
		// |browser_view| may be NULL, in which case SetBrowserView() will be called.
		ViewsWindow(Delegate* delegate,
			CefRefPtr<CefBrowserView> browser_view);

		void SetBrowserView(CefRefPtr<CefBrowserView> browser_view);


		Delegate* Del;  // Not owned by this object.
		CefRefPtr<CefBrowserView> BrowserView;
		bool Frameless;
		CefRefPtr<CefWindow> Window;

		CefSize MinWindowSize;

		IMPLEMENT_REFCOUNTING(ViewsWindow);
		DISALLOW_COPY_AND_ASSIGN(ViewsWindow);
	};
}