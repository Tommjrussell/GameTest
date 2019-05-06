#include "Engine.h"

sal::App::App(std::unique_ptr<AppDriver>&& driver)
	: m_driver(std::move(driver))
{
	m_driver->SetApp(this);
}

bool sal::App::Run(const sal::WindowDetails& details)
{
	_ASSERT(bool(m_driver));
	return m_driver->Run(details);
}

void sal::App::Init()
{
	RegisterState(
		kMasterStateId,
		-1,
		&MasterState::Create,
		nullptr);

	OnStartup();

	PushState(kMasterStateId);
}

bool sal::App::Frame(double elapsedTime)
{
	if (m_stateStack.empty())
		return false;

	HandleMessages();

	m_stateStack.back()->Update(elapsedTime);

	DoStateChange();
	return true;
}

void sal::App::Render()
{
	if (!m_stateRendererStack.empty())
	{
		m_stateRendererStack.back()->Render();
	}
}

void sal::App::KeyboardInput(bool down, int keyCode)
{
	if (!m_stateRendererStack.empty())
	{
		m_stateRendererStack.back()->OnKeyboardInput(down, keyCode);
	}
}

void sal::App::KeyCharacter(int keyCode, int modifiers)
{
	if (!m_stateRendererStack.empty())
	{
		m_stateRendererStack.back()->OnKeyCharacter(keyCode, modifiers);
	}
}

void sal::App::MouseInput(bool down, int mouseButtonCode)
{
	if (!m_stateRendererStack.empty())
	{
		m_stateRendererStack.back()->OnMouseInput(down, mouseButtonCode);
	}
}

void sal::App::MouseMove(const Point2D& mouseXY)
{
	if (!m_stateRendererStack.empty())
	{
		m_stateRendererStack.back()->OnMouseMove(mouseXY);
	}
}

void sal::App::Cleanup()
{
	// TODO : if it is found that nothing actually needs to be done at this stage, this function can be removed
	// and a direct call to OnShutdown() made.
	OnShutdown();

	while (!m_stateRendererStack.empty())
		m_stateRendererStack.pop_back();
}

void sal::App::PushState(int stateId)
{
	_ASSERT(stateId >= 0 && stateId < int(m_stateTable.size()));
	const auto parentState = m_stateStack.empty()
		? (State*)nullptr
		: m_stateStack.back().get();
	{
		auto state = m_stateTable[stateId].createFunc(*this, parentState);
		_ASSERT(bool(state));
		_ASSERT(state->GetStateId() == stateId);
		m_stateStack.push_back(std::move(state));
	}
	const auto parentStateRenderer = m_stateRendererStack.empty()
		? (StateRenderer*)nullptr
		: m_stateRendererStack.back().get();
	{
		auto stateRenderer = m_stateTable[stateId].createRendererFunc(
			m_driver->m_viewIO.get(), m_stateStack.back().get(), parentStateRenderer);

		if (bool(stateRenderer))
		{
			m_stateRendererStack.push_back(std::move(stateRenderer));
			m_stateStack.back()->Init(/*m_stateRendererStack.back().get(), m_stateParams*/);
			m_stateRendererStack.back()->Init();
		}
		else
		{
			m_stateStack.back()->Init(/*nullptr, m_stateParams*/);
		}

		// m_stateParams must be emptied by the State's 'Init' function even
		// if it is uninterested in the parameters.
		//_ASSERT(m_stateParams.empty());
	}

	const auto initialChildStateId = m_stateTable[stateId].initialChildStateId;
	if (initialChildStateId != -1)
	{
		PushState(initialChildStateId);
	}
}

void sal::App::PopState()
{
	if (!m_stateRendererStack.empty()
		&& m_stateStack.back().get() == m_stateRendererStack.back()->GetState())
	{
		m_stateRendererStack.pop_back();
		if (!m_stateRendererStack.empty())
			m_stateRendererStack.back()->Focus();
	}

	m_stateStack.back()->Finish();
	m_stateStack.pop_back();
}

void sal::App::DoStateChange()
{
	if (m_pendingStateChange == -1)
		return;

	PopState();

	while (!m_stateStack.empty() && !IsChildState(m_stateStack.back()->GetStateId(), m_pendingStateChange))
	{
		PopState();
	}

	if (m_stateStack.empty())
	{
		//m_finished = true;
		return;
	}

	PushState(m_pendingStateChange);
	m_pendingStateChange = -1;
}

bool sal::App::IsChildState(int stateId, int childState) const
{
	_ASSERT(stateId >= 0 && stateId < int(m_stateTable.size()));
	_ASSERT(childState >= 0 && childState < int(m_stateTable.size()));

	return (m_stateTable[childState].parentStateId == stateId);
}

void sal::App::HandleMessages()
{
	auto& mq = m_messages.m_messageQueue;

	while (!mq.empty())
	{
		auto& m = mq.front();

		auto rit = m_stateStack.rbegin();
		for (; rit != m_stateStack.rend(); ++rit)
		{
			if ((*rit)->HandleMessage(m))
			{
				break;
			}
		}

		mq.pop_front();
	}
}

void sal::App::RegisterState(int stateId, int parentStateId,
	CreateStateFunc createFunc, CreateStateRendererFunc createRendererFunc)
{
	_ASSERT(stateId >= 0);
	_ASSERT(parentStateId == -1
		|| (parentStateId >= 0 && parentStateId < int(m_stateTable.size())));

	if (size_t(stateId) >= m_stateTable.size())
	{
		m_stateTable.resize(size_t(stateId) + 1);
	}

	RegisteredState& rState = m_stateTable[stateId];

	rState.parentStateId = parentStateId;

	rState.initialChildStateId = -1;
	rState.createFunc = createFunc;

	if (createRendererFunc)
	{
		rState.createRendererFunc = createRendererFunc;
	}
	else
	{
		rState.createRendererFunc = [](ViewIO*, State*, StateRenderer*) -> std::unique_ptr<StateRenderer> { return nullptr; };
	}

	// Set first registered child of a state to be the initial child of that state.
	if (parentStateId != -1 && m_stateTable[parentStateId].initialChildStateId == -1)
	{
		m_stateTable[parentStateId].initialChildStateId = stateId;
	}
}
