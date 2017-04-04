#include "dllmain.h"
#include "rio.h"
#include "Privilege.hpp"

namespace EternalStrings {
#define createELit(eLit) v8::Eternal<v8::String> es_##eLit;
#define createEStr(eVar, eStr) v8::Eternal<v8::String> es_##eVar;

#include "EternalStrings.h"
#undef createELit
#undef createEStr
}

#pragma comment(lib, 	"Ws2_32.lib")

#include "UdpSocket.hpp"

v8::Eternal<v8::Function> UdpSocket::constructor;
Sockets::GuidMsTcpIp UdpSocket::guidMsTcpIP;

namespace {
	BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
		switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
		}

		return TRUE;
	}

	void Init(v8::Local<v8::Object> exports) {
		Sockets::Win10SocketLib win10SocketLib;
	//Privilege(TEXT("SeLockMemoryPrivilege"), TRUE);
	//Privilege(TEXT("SeCreateGlobalPrivilege"), TRUE);


		auto isolate{ v8::Isolate::GetCurrent() };
		v8::HandleScope handle_scope{ isolate };

		{
#define createELit(eLit) { EternalStrings::es_##eLit.Set(isolate, v8::String::NewFromTwoByte(isolate, ptr_to_ptr<const wchar_t *, const uint16_t *>(L#eLit), v8::String::kInternalizedString)); }
#define createEStr(eVar, eStr) { EternalStrings::es_##eVar.Set(isolate, v8::String::NewFromTwoByte(isolate, ptr_to_ptr<const wchar_t *, const uint16_t *>(L#eStr), v8::String::kInternalizedString)); }
#include "EternalStrings.h"
#undef createELit
#undef createEStr
		}

#ifdef _DEBUG
		node::AtExit([](void*) {
			puts("\n\nAtExit_DebugCallback()\nPress any key to exit...");
			getchar();
		});
#endif

		UdpSocket::Init(exports);
#if 0
		ThreadPromise::Init(target);
		setMethod(target, L"CreateThreadPromise", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			ThreadPromise::NewInstance(info);
		});
#endif

#if 0

		Object socket
		createSocket

		symbol Event::close
		Object Error
		symbol Event::listening
		Object Message
		
		*/

		/*setMethod(dgram, L"createSocketUdp", [](const v8::FunctionCallbackInfo<v8::Value>& info) {
			v8::Isolate* isolate = v8::Isolate::GetCurrent();
			v8::HandleScope scope(isolate);
		//Sockets::RioUdp4
			/*
			if (info.Length() < 3) { info.GetReturnValue().SetUndefined(); return; }

			auto str_udp4 = EternalStrings::es_udp4.Get(isolate);
			auto str_udp6 = EternalStrings::es_udp6.Get(isolate);
			auto str_type = EternalStrings::es_type.Get(isolate);
			if (info[0]->IsString()) {
				auto argType = info[0]->ToString();
				if (argType->StrictEquals(str_udp4)) {
					info.GetReturnValue().Set(str_udp4);
					return;
				}
				else if (argType->StrictEquals(str_udp6)) {
					info.GetReturnValue().Set(str_udp6);
					return;
				}
			}
			else if (info[0]->IsObject()) {
				Local<Context> context = isolate->GetCurrentContext();
				auto argOptions = info[0]->ToObject();
				if (!argOptions->HasOwnProperty(context, str_type).ToChecked()) {
					info.GetReturnValue().SetUndefined();
					return;
				}
				auto argType = argOptions->Get(str_type)->ToString();
				if (argType->StrictEquals(str_udp4)) {
					info.GetReturnValue().Set(str_udp4);
					return;
				} else if (argType->StrictEquals(str_udp6)) {
					info.GetReturnValue().Set(str_udp6);
					return;
				}
			}
			info.GetReturnValue().SetNull();
			return;
			*/
		});

		/*
		LPFN_RIORECEIVE               RIOReceive;
		LPFN_RIORECEIVEEX             RIOReceiveEx;
		LPFN_RIOSEND                  RIOSend;
		LPFN_RIOSENDEX                RIOSendEx;
		LPFN_RIOCLOSECOMPLETIONQUEUE  RIOCloseCompletionQueue;
		LPFN_RIOCREATECOMPLETIONQUEUE RIOCreateCompletionQueue;
		LPFN_RIOCREATEREQUESTQUEUE    RIOCreateRequestQueue;
		LPFN_RIODEQUEUECOMPLETION     RIODequeueCompletion;
		LPFN_RIODEREGISTERBUFFER      RIODeregisterBuffer;
		LPFN_RIONOTIFY                RIONotify;
		LPFN_RIOREGISTERBUFFER        RIORegisterBuffer;
		LPFN_RIORESIZECOMPLETIONQUEUE RIOResizeCompletionQueue;
		LPFN_RIORESIZEREQUESTQUEUE    RIOResizeRequestQueue;


		*/
#endif

	}

	NODE_MODULE(addon, Init)
}
