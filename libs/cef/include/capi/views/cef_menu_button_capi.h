// Copyright (c) 2018 Marshall A. Greenblatt. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the name Chromium Embedded
// Framework nor the names of its contributors may be used to endorse
// or promote products derived from this software without specific prior
// written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool and should not edited
// by hand. See the translator.README.txt file in the tools directory for
// more information.
//
// $hash=2ec56bd536038f39754627b528e9b371660c6aa7$
//

#ifndef CEF_INCLUDE_CAPI_VIEWS_CEF_MENU_BUTTON_CAPI_H_
#define CEF_INCLUDE_CAPI_VIEWS_CEF_MENU_BUTTON_CAPI_H_
#pragma once

#include "include/capi/cef_menu_model_capi.h"
#include "include/capi/views/cef_label_button_capi.h"
#include "include/capi/views/cef_menu_button_delegate_capi.h"

#ifdef __cplusplus
extern "C" {
#endif

///
// MenuButton is a button with optional text, icon and/or menu marker that shows
// a menu when clicked with the left mouse button. All size and position values
// are in density independent pixels (DIP) unless otherwise indicated. Methods
// must be called on the browser process UI thread unless otherwise indicated.
///
typedef struct _cef_menu_button_t {
  ///
  // Base structure.
  ///
  cef_label_button_t base;

  ///
  // Show a menu with contents |menu_model|. |screen_point| specifies the menu
  // position in screen coordinates. |anchor_position| specifies how the menu
  // will be anchored relative to |screen_point|. This function should be called
  // from cef_menu_button_delegate_t::on_menu_button_pressed().
  ///
  void(CEF_CALLBACK* show_menu)(struct _cef_menu_button_t* self,
                                struct _cef_menu_model_t* menu_model,
                                const cef_point_t* screen_point,
                                cef_menu_anchor_position_t anchor_position);

  ///
  // Show the menu for this button. Results in a call to
  // cef_menu_button_delegate_t::on_menu_button_pressed().
  ///
  void(CEF_CALLBACK* trigger_menu)(struct _cef_menu_button_t* self);
} cef_menu_button_t;

///
// Create a new MenuButton. A |delegate| must be provided to call show_menu()
// when the button is clicked. |text| will be shown on the MenuButton and used
// as the default accessible name. If |with_frame| is true (1) the button will
// have a visible frame at all times, center alignment, additional padding and a
// default minimum size of 70x33 DIP. If |with_frame| is false (0) the button
// will only have a visible frame on hover/press, left alignment, less padding
// and no default minimum size. If |with_menu_marker| is true (1) a menu marker
// will be added to the button.
///
CEF_EXPORT cef_menu_button_t* cef_menu_button_create(
    struct _cef_menu_button_delegate_t* delegate,
    const cef_string_t* text,
    int with_frame,
    int with_menu_marker);

#ifdef __cplusplus
}
#endif

#endif  // CEF_INCLUDE_CAPI_VIEWS_CEF_MENU_BUTTON_CAPI_H_
