// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mojo/edk/system/test/test_io_thread.h"

#include "base/bind.h"
#include "mojo/edk/util/waitable_event.h"

using mojo::util::AutoResetWaitableEvent;

namespace mojo {
namespace system {
namespace test {

namespace {

void PostTaskAndWaitHelper(AutoResetWaitableEvent* event,
                           const base::Closure& task) {
  task.Run();
  event->Signal();
}

}  // namespace

TestIOThread::TestIOThread(StartMode start_mode)
    : io_thread_("test_io_thread"), io_thread_started_(false) {
  switch (start_mode) {
    case StartMode::AUTO:
      Start();
      return;
    case StartMode::MANUAL:
      return;
  }
  CHECK(false) << "Invalid mode";
}

TestIOThread::~TestIOThread() {
  Stop();
}

void TestIOThread::Start() {
  CHECK(!io_thread_started_);
  io_thread_started_ = true;
  CHECK(io_thread_.StartWithOptions(
      base::Thread::Options(base::MessageLoop::TYPE_IO, 0)));
}

void TestIOThread::Stop() {
  // Note: It's okay to call |Stop()| even if the thread isn't running.
  io_thread_.Stop();
  io_thread_started_ = false;
}

void TestIOThread::PostTask(const base::Closure& task) {
  embedder::PlatformPostTask(task_runner().get(), task);
}

void TestIOThread::PostTaskAndWait(const base::Closure& task) {
  AutoResetWaitableEvent event;
  embedder::PlatformPostTask(task_runner().get(),
                             base::Bind(&PostTaskAndWaitHelper, &event, task));
  event.Wait();
}

}  // namespace test
}  // namespace system
}  // namespace mojo