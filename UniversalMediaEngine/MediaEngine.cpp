﻿#include "pch.h"
#include "MediaEngineManager.h"
#include "MediaEngine.h"

using namespace UniversalMediaEngine;
using namespace Platform;
using namespace concurrency;

UniversalMediaEngine::MediaEngine::~MediaEngine()
{
	if (nullptr != spMediaEngineManager.Get())
	{
		spMediaEngineManager->TearDown();
	}
}

IAsyncOperation<MediaEngineInitializationResult>^ MediaEngine::InitializeAsync()
{
	return create_async([this]
	{
		spMediaEngineManager = new MediaEngineManager(this);
		if (spMediaEngineManager.Get() != nullptr)
		{
			HRESULT hr = spMediaEngineManager->Initialize();
			if (SUCCEEDED(hr))
			{
				return MediaEngineInitializationResult::Success;
			}
		}

		return MediaEngineInitializationResult::Fail;
	});
}

void MediaEngine::Play(Platform::String^ url)
{
	HRESULT hr = E_FAIL;
	BSTR bstrUrl;

	bstrUrl = SysAllocString(url->Data());

	hr = spMediaEngineManager->PlayUrl(bstrUrl);

	SysFreeString(bstrUrl);

	// Check the HR at the end so we know the string is freed before throw
	CHECK_INIT(hr);
}

void MediaEngine::Pause()
{
	CHECK_INIT(spMediaEngineManager->Pause());
}

void UniversalMediaEngine::MediaEngine::Stop()
{
	CHECK_INIT(spMediaEngineManager->Stop());
}

double MediaEngine::Volume::get()
{
	double volume;
	CHECK_INIT(spMediaEngineManager->GetVolume(&volume));
	return volume;
}

void MediaEngine::Volume::set(double value)
{
	CHECK_INIT(spMediaEngineManager->SetVolume(value));
}

void MediaEngine::TriggerMediaStateChanged(UniversalMediaEngine::MediaState state)
{
	MediaStateChanged(state);
}