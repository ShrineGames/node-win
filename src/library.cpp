// clang-format off
#include <napi.h>

#include "input.h"
#include "service.h"
// clang-format on

Napi::Number GetTimeSinceLastInput(const Napi::CallbackInfo& info)
{
  Napi::Env env = info.Env();

  int idle_ms = Input::GetTimeSinceLastInput();
  return Napi::Number::New(env, idle_ms);
}

Napi::Boolean Service_Install(const Napi::CallbackInfo& info)
{
  Napi::Env env = info.Env();

  auto serviceName = info[0].As<Napi::String>().Utf8Value();
  auto displayName = info[1].As<Napi::String>().Utf8Value();
  auto fileName = info[2].As<Napi::String>().Utf8Value();

  auto installed = Service::Install(serviceName, displayName, fileName);
  return Napi::Boolean::New(env, installed);
}

Napi::Boolean Service_Uninstall(const Napi::CallbackInfo& info)
{
  Napi::Env env = info.Env();

  auto serviceName = info[0].As<Napi::String>().Utf8Value();

  auto uninstalled = Service::Uninstall(serviceName);
  return Napi::Boolean::New(env, uninstalled);
}

Napi::Boolean Service_IsInstalled(const Napi::CallbackInfo& info)
{
  Napi::Env env = info.Env();

  auto serviceName = info[0].As<Napi::String>().Utf8Value();

  auto isInstalled = Service::IsInstalled(serviceName);
  return Napi::Boolean::New(env, isInstalled);
}

Napi::Boolean Service_Start(const Napi::CallbackInfo& info)
{
  Napi::Env env = info.Env();

  auto serviceName = info[0].As<Napi::String>().Utf8Value();

  auto started = Service::Start(serviceName);
  return Napi::Boolean::New(env, started);
}

Napi::Boolean Service_Stop(const Napi::CallbackInfo& info)
{
  Napi::Env env = info.Env();

  auto serviceName = info[0].As<Napi::String>().Utf8Value();

  auto stopped = Service::Stop(serviceName);
  return Napi::Boolean::New(env, stopped);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "GetTimeSinceLastInput"),
              Napi::Function::New(env, GetTimeSinceLastInput));
  exports.Set(Napi::String::New(env, "Service_Install"),
              Napi::Function::New(env, Service_Install));
  exports.Set(Napi::String::New(env, "Service_Uninstall"),
              Napi::Function::New(env, Service_Uninstall));
  exports.Set(Napi::String::New(env, "Service_IsInstalled"),
              Napi::Function::New(env, Service_IsInstalled));
  exports.Set(Napi::String::New(env, "Service_Start"),
              Napi::Function::New(env, Service_Start));
  exports.Set(Napi::String::New(env, "Service_Stop"),
              Napi::Function::New(env, Service_Stop));

  return exports;
}

NODE_API_MODULE(addon, Init)