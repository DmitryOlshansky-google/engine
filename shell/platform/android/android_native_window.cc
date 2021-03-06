// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/shell/platform/android/android_native_window.h"

namespace shell {

AndroidNativeWindow::AndroidNativeWindow(Handle window) : window_(window) {
  if (window_ != nullptr) {
    ANativeWindow_acquire(window_);
  }
}

AndroidNativeWindow::AndroidNativeWindow(AndroidNativeWindow&& o)
    : window_(o.window_) {
  o.window_ = nullptr;
}

AndroidNativeWindow::~AndroidNativeWindow() {
  if (window_ != nullptr) {
    ANativeWindow_release(window_);
    window_ = nullptr;
  }
}

bool AndroidNativeWindow::IsValid() const {
  return window_ != nullptr;
}

AndroidNativeWindow::Handle AndroidNativeWindow::handle() const {
  return window_;
}

}  // namespace shell
