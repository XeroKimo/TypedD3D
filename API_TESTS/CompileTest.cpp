#include "pch.h"
#include <d3d11_4.h>
#include <concepts>
#include <d3d12.h>

import TypedD3D.Shared;
import TypedD3D11;
import TypedD3D12;

using namespace TypedD3D;

static void ContainersTest()
{
	static_assert(IUnknownWrapper<WeakWrapper<Untagged<ID3D11Device>>>);
	static_assert(IUnknownWrapper<ElementReference<WeakWrapper<Untagged<ID3D11Device>>, false>>);
	static_assert(IUnknownWrapper<ElementReference<WeakWrapper<Untagged<ID3D11Device>>, true>>);

	static_assert(IUnknownWeakWrapper<WeakWrapper<Untagged<ID3D11Device>>>);
	static_assert(IUnknownWeakWrapper<ElementReference<WeakWrapper<Untagged<ID3D11Device>>, false>>);
	static_assert(IUnknownWeakWrapper<ElementReference<WeakWrapper<Untagged<ID3D11Device>>, true>>);

	static_assert(IUnknownWrapper<StrongWrapper<Untagged<ID3D11Device>>>);
	static_assert(IUnknownWrapper<ElementReference<StrongWrapper<Untagged<ID3D11Device>>, false>>);
	static_assert(IUnknownWrapper<ElementReference<StrongWrapper<Untagged<ID3D11Device>>, true>>);

	static_assert(std::constructible_from<StrongWrapper<Untagged<ID3D11Device>>, WeakWrapper<Untagged<ID3D11Device>>>);
	static_assert(std::constructible_from<WeakWrapper<Untagged<ID3D11Device>>, StrongWrapper<Untagged<ID3D11Device>>>);

	static_assert(std::constructible_from<StrongWrapper<Untagged<ID3D11Device>>, WeakWrapper<Untagged<ID3D11Device5>>>);
	static_assert(std::constructible_from<WeakWrapper<Untagged<ID3D11Device>>, StrongWrapper<Untagged<ID3D11Device5>>>);


	ID3D11Device5* raw;
	WeakWrapper<Untagged<ID3D11Device5>> weak = raw;
	WeakWrapper<Untagged<ID3D11Device>> weak2 = weak;
	weak2 = weak;
	weak = Cast<ID3D11Device5>(weak2);
	const WeakWrapper<Untagged<ID3D11Device>>& weak3 = weak;
	const WeakWrapper<Untagged<ID3D11Device>>& weak4 = weak2;
	weak = raw;
	weak = Cast<ID3D11Device5>(raw);
	weak2 = raw;

	///Shouldn't compile
	//weak3 = raw;
	//weak4 = raw;

	weak == weak2;
	weak3 == weak4;
	weak == raw;
	weak2 == raw;
	weak2 == weak;
	raw == weak;
	raw == weak2;
	weak == nullptr;
	nullptr == weak;
	weak2 == nullptr;
	nullptr == weak;

	weak.Get();
	weak.Acquire();
	weak.Release();

	weak.Attach(nullptr);
	weak.Attach(raw);
	weak.Detach();
	weak2.Attach(raw);

	weak3.Get();
	weak3.Acquire();
	weak3.Release();

	///Shouldn't compile
	//weak3.Attach(nullptr);
	//weak3.Attach(raw);
	//weak3.Detach();

	weak2->CheckCounterInfo();
	(*weak2).CheckCounterInfo();
	weak3->CheckCounterInfo();
	(*weak3).CheckCounterInfo();

	StrongWrapper<Untagged<ID3D11Device5>> strong = raw;
	StrongWrapper<Untagged<ID3D11Device>> strong2 = strong;
	strong2 = strong;
	strong = Cast<ID3D11Device5>(strong2);
	const StrongWrapper<Untagged<ID3D11Device>>& strong3 = strong;
	const StrongWrapper<Untagged<ID3D11Device>>& strong4 = strong2;
	strong = raw;
	strong = Cast<ID3D11Device5>(raw);
	strong2 = raw;

	///Shouldn't compile
	//strong3 = raw;
	//strong3 = strong2;
	//strong4 = raw;

	strong == strong2;
	strong3 == strong4;
	strong == raw;
	strong2 == raw;
	strong2 == strong;
	raw == strong;
	raw == strong2;
	strong == nullptr;
	nullptr == strong;
	strong2 == nullptr;
	nullptr == strong;

	strong.Get();

	strong.Attach(nullptr);
	strong.Attach(raw);
	strong.Detach();
	strong2.Attach(raw);

	strong3.Get();

	///Shouldn't compile
	//strong3.Attach(nullptr);
	//strong3.Attach(raw);
	//strong3.Detach();

	strong2->CheckCounterInfo();
	(*strong2).CheckCounterInfo();
	strong3->CheckCounterInfo();
	(*strong3).CheckCounterInfo();

	weak2 = strong;
	weak2 = strong2;
	strong2 = weak;
	strong2 = weak2;

	WeakWrapper<Untagged<ID3D11Device>> weak5 = strong;
	WeakWrapper<Untagged<ID3D11Device>> weak6 = strong2;
	StrongWrapper<Untagged<ID3D11Device>> strong5 = weak;
	StrongWrapper<Untagged<ID3D11Device>> strong6 = weak2;

	Array<WeakWrapper<Untagged<ID3D11Device5>>, 2> weakArr = { raw, raw };
	Array<WeakWrapper<Untagged<ID3D11Device>>, 2> weakArr2 = { weakArr[0], raw };
	weakArr[0] = Cast<ID3D11Device5>(weakArr[1]);
	const Array<WeakWrapper<Untagged<ID3D11Device>>, 2>& weakArr3 = weak;
	Array<WeakWrapper<Untagged<ID3D11Device>>, 2>& weakArr4 = weakArr2;
	weakArr[0] = raw;
	weakArr[0] = Cast<ID3D11Device5>(raw);
	weakArr2[0] = raw;

	///Shouldn't compile
	//weakArr3[0] = raw;
	//weakArr3[0] = weakArr2[0];

	weakArr[0] == weakArr2[0];
	weakArr[0] == raw;
	weakArr2[0] == raw;
	weakArr2[0] == weakArr[0];
	raw == weakArr[0];
	raw == weakArr2[0];
	weakArr[0] == nullptr;
	nullptr == weakArr[0];
	weakArr2[0] == nullptr;
	nullptr == weakArr[0];

	weakArr3[0] == weakArr[0];
	weakArr3[0] == raw;
	weakArr3[0] == nullptr;
	weakArr[0] == weakArr3[0];
	raw == weakArr3[0];
	nullptr == weakArr3[0];

	weakArr[0].Get();
	weakArr[0].Acquire();
	weakArr[0].Release();

	weakArr[0].Attach(nullptr);
	weakArr[0].Attach(raw);
	weakArr[0].Detach();
	weakArr2[0].Attach(raw);

	weakArr3[0].Get();
	weakArr3[0].Acquire();
	weakArr3[0].Release();

	///Shouldn't compile
	//weakArr3[0].Attach(nullptr);
	//weakArr3[0].Attach(raw);
	//weakArr3[0].Detach();

	weakArr2[0]->CheckCounterInfo();
	(*weakArr2[0]).CheckCounterInfo();
	weakArr3[0]->CheckCounterInfo();
	(*weakArr3[0]).CheckCounterInfo();

	Array<StrongWrapper<Untagged<ID3D11Device5>>, 2> strongArr = { raw, raw };
	Array<StrongWrapper<Untagged<ID3D11Device>>, 2> strongArr2 = { strongArr[0], strongArr[1] };
	strongArr[0] = Cast<ID3D11Device5>(strongArr[1]);
	const Array<StrongWrapper<Untagged<ID3D11Device>>, 2>& strongArr3 = strong;
	Array<StrongWrapper<Untagged<ID3D11Device>>, 2>& strongArr4 = strongArr2;
	Array strongArr5{ strong, strong };
	Array strongArr6{ strongArr[0], strong };
	strongArr[0] = raw;
	strongArr[0] = Cast<ID3D11Device5>(raw);
	strongArr2[0] = raw;

	///Shouldn't compile
	//strongArr3[0] = raw;
	//strongArr3[0] = strongArr2[0];

	strongArr[0] == strongArr2[0];
	strongArr[0] == raw;
	strongArr2[0] == raw;
	strongArr2[0] == strongArr[0];
	raw == strongArr[0];
	raw == strongArr2[0];
	strongArr[0] == nullptr;
	nullptr == strongArr[0];
	strongArr2[0] == nullptr;
	nullptr == strongArr[0];

	strongArr3[0] == strongArr[0];
	strongArr3[0] == raw;
	strongArr3[0] == nullptr;
	strongArr[0] == strongArr3[0];
	raw == strongArr3[0];
	nullptr == strongArr3[0];

	strongArr[0].Get();

	strongArr[0].Attach(nullptr);
	strongArr[0].Attach(raw);
	strongArr[0].Detach();
	strongArr2[0].Attach(raw);

	strongArr3[0].Get();

	///Shouldn't compile
	//strongArr3[0].Attach(nullptr);
	//strongArr3[0].Attach(raw);
	//strongArr3[0].Detach();

	strongArr2[0]->CheckCounterInfo();
	(*strongArr2[0]).CheckCounterInfo();
	strongArr3[0]->CheckCounterInfo();
	(*strongArr3[0]).CheckCounterInfo();

	weak = weakArr[0];
	weak = std::move(weakArr)[0];
	weakArr[0] = weak;
	weakArr[0] = std::move(weak);

	weak2 = weakArr[0];
	weak2 = std::move(weakArr)[0];
	weakArr[0] = Cast<ID3D11Device5>(weak2);
	weakArr[0] = std::move(Cast<ID3D11Device5>(weak2));
	WeakWrapper<Untagged<ID3D11Device5>> weak7 = weakArr[0];
	WeakWrapper<Untagged<ID3D11Device5>> weak8 = std::move(weakArr)[0];
	WeakWrapper<Untagged<ID3D11Device>> weak9 = weakArr[0];
	WeakWrapper<Untagged<ID3D11Device>> weak10 = std::move(weakArr)[0];
	WeakWrapper<Untagged<ID3D11Device>> weak11 = weakArr2[0];
	WeakWrapper<Untagged<ID3D11Device>> weak12 = std::move(weakArr2)[0];

	strong = strongArr[0];
	strong = std::move(strongArr)[0];
	strongArr[0] = strong;
	strongArr[0] = std::move(strong);

	strong2 = strongArr[0];
	strong2 = std::move(strongArr)[0];
	strongArr[0] = Cast<ID3D11Device5>(strong2);
	strongArr[0] = std::move(Cast<ID3D11Device5>(strong2));
	StrongWrapper<Untagged<ID3D11Device5>> strong7 = strongArr[0];
	StrongWrapper<Untagged<ID3D11Device5>> strong8 = std::move(strongArr)[0];
	StrongWrapper<Untagged<ID3D11Device>> strong9 = strongArr[0];
	StrongWrapper<Untagged<ID3D11Device>> strong10 = std::move(strongArr)[0];
	StrongWrapper<Untagged<ID3D11Device>> strong11 = strongArr2[0];
	StrongWrapper<Untagged<ID3D11Device>> strong12 = std::move(strongArr2)[0];

	strongArr[0] = weakArr[0];
	weakArr[0] = strongArr[0];
	strongArr[0] = std::move(weakArr)[0];
	weakArr[0] = std::move(strongArr)[0];
	weakArr[0] == strongArr[0];

	strong == weak;
	weak == strong;

	for(StrongWrapper<Untagged<ID3D11Device5>> values : strongArr)
	{

	}

	for(auto values : strongArr)
	{

	}

	Span s = strongArr;
	Span s2 = s;
	Span<const StrongWrapper<Untagged<ID3D11Device5>>, 2> s3 = strongArr;
	Span<const StrongWrapper<Untagged<ID3D11Device5>>, 2> s4 = s2;
	Span s5{ s2.begin(), s2.end() };
	Span s6{ s2.begin(), s2.size() };
	s = s2;
	s = strongArr;
	s3 = s;
	s3 = s4;
	s3 = strongArr;

	Span<StrongWrapper<Untagged<ID3D11Device5>>> s7 = strongArr;
	Span<StrongWrapper<Untagged<ID3D11Device5>>> s8 = s7;
	Span<StrongWrapper<Untagged<ID3D11Device5>>> s9 = s2;

	Vector v{ strong, strong };
	Vector v2{ strongArr[0], strong };
	Vector v3{ strong, strongArr[0] };

	v.empty();
	v.size();
	s.size();
	v.push_back(nullptr);
	v.pop_back();


	for(StrongWrapper<Untagged<ID3D11Device5>> values : s)
	{

	}

	for(auto values : s)
	{

	}

	for(StrongWrapper<Untagged<ID3D11Device5>> values : v)
	{

	}

	for(auto values : v)
	{

	}

	auto SpanInput = [](Span<StrongWrapper<Untagged<ID3D11Device5>>>) {};
	auto SpanInput2 = [](Span<const StrongWrapper<Untagged<ID3D11Device5>>>) {};
	SpanInput(strongArr);
	SpanInput2(strongArr);
	SpanInput(s);
	SpanInput2(s);
	//SpanInput(s3); //Shouldn't compile
	SpanInput2(s3);
	SpanInput(s5);
	SpanInput2(s5);

	Array<RTV<D3D12_CPU_DESCRIPTOR_HANDLE>, 2> cpuHandle;
	const Array<RTV<D3D12_CPU_DESCRIPTOR_HANDLE>, 2> cpuHandle2;

	auto SpanInput3 = [](Span<RTV<D3D12_CPU_DESCRIPTOR_HANDLE>>) {};
	auto SpanInput4 = [](Span<const RTV<D3D12_CPU_DESCRIPTOR_HANDLE>>) {};
	SpanInput3(cpuHandle);
	SpanInput4(cpuHandle);

	//SpanInput3(cpuHandle2); //shouldn't compile
	SpanInput4(cpuHandle2);
}