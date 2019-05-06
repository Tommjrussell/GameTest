#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <deque>

namespace sal
{
	enum MouseButton
	{
		SAL_MOUSEBUTTON_INVALID = -1,
		SAL_MOUSEBUTTON_LEFT = 0,
		SAL_MOUSEBUTTON_RIGHT = 1,
		SAL_MOUSEBUTTON_MIDDLE = 2,
	};

	// key codes are the same names and values as GLFW & SAPP
	enum Keycode
	{
		SAL_KEYCODE_INVALID = 0,
		SAL_KEYCODE_SPACE = 32,
		SAL_KEYCODE_APOSTROPHE = 39,
		SAL_KEYCODE_COMMA = 44,
		SAL_KEYCODE_MINUS = 45,
		SAL_KEYCODE_PERIOD = 46,
		SAL_KEYCODE_SLASH = 47,
		SAL_KEYCODE_0 = 48,
		SAL_KEYCODE_1 = 49,
		SAL_KEYCODE_2 = 50,
		SAL_KEYCODE_3 = 51,
		SAL_KEYCODE_4 = 52,
		SAL_KEYCODE_5 = 53,
		SAL_KEYCODE_6 = 54,
		SAL_KEYCODE_7 = 55,
		SAL_KEYCODE_8 = 56,
		SAL_KEYCODE_9 = 57,
		SAL_KEYCODE_SEMICOLON = 59,
		SAL_KEYCODE_EQUAL = 61,
		SAL_KEYCODE_A = 65,
		SAL_KEYCODE_B = 66,
		SAL_KEYCODE_C = 67,
		SAL_KEYCODE_D = 68,
		SAL_KEYCODE_E = 69,
		SAL_KEYCODE_F = 70,
		SAL_KEYCODE_G = 71,
		SAL_KEYCODE_H = 72,
		SAL_KEYCODE_I = 73,
		SAL_KEYCODE_J = 74,
		SAL_KEYCODE_K = 75,
		SAL_KEYCODE_L = 76,
		SAL_KEYCODE_M = 77,
		SAL_KEYCODE_N = 78,
		SAL_KEYCODE_O = 79,
		SAL_KEYCODE_P = 80,
		SAL_KEYCODE_Q = 81,
		SAL_KEYCODE_R = 82,
		SAL_KEYCODE_S = 83,
		SAL_KEYCODE_T = 84,
		SAL_KEYCODE_U = 85,
		SAL_KEYCODE_V = 86,
		SAL_KEYCODE_W = 87,
		SAL_KEYCODE_X = 88,
		SAL_KEYCODE_Y = 89,
		SAL_KEYCODE_Z = 90,
		SAL_KEYCODE_LEFT_BRACKET = 91,
		SAL_KEYCODE_BACKSLASH = 92,
		SAL_KEYCODE_RIGHT_BRACKET = 93,
		SAL_KEYCODE_GRAVE_ACCENT = 96,  // `
		SAL_KEYCODE_WORLD_1 = 161, // non-US #1
		SAL_KEYCODE_WORLD_2 = 162, // non-US #2
		SAL_KEYCODE_ESCAPE = 256,
		SAL_KEYCODE_ENTER = 257,
		SAL_KEYCODE_TAB = 258,
		SAL_KEYCODE_BACKSPACE = 259,
		SAL_KEYCODE_INSERT = 260,
		SAL_KEYCODE_DELETE = 261,
		SAL_KEYCODE_RIGHT = 262,
		SAL_KEYCODE_LEFT = 263,
		SAL_KEYCODE_DOWN = 264,
		SAL_KEYCODE_UP = 265,
		SAL_KEYCODE_PAGE_UP = 266,
		SAL_KEYCODE_PAGE_DOWN = 267,
		SAL_KEYCODE_HOME = 268,
		SAL_KEYCODE_END = 269,
		SAL_KEYCODE_CAPS_LOCK = 280,
		SAL_KEYCODE_SCROLL_LOCK = 281,
		SAL_KEYCODE_NUM_LOCK = 282,
		SAL_KEYCODE_PRINT_SCREEN = 283,
		SAL_KEYCODE_PAUSE = 284,
		SAL_KEYCODE_F1 = 290,
		SAL_KEYCODE_F2 = 291,
		SAL_KEYCODE_F3 = 292,
		SAL_KEYCODE_F4 = 293,
		SAL_KEYCODE_F5 = 294,
		SAL_KEYCODE_F6 = 295,
		SAL_KEYCODE_F7 = 296,
		SAL_KEYCODE_F8 = 297,
		SAL_KEYCODE_F9 = 298,
		SAL_KEYCODE_F10 = 299,
		SAL_KEYCODE_F11 = 300,
		SAL_KEYCODE_F12 = 301,
		SAL_KEYCODE_F13 = 302,
		SAL_KEYCODE_F14 = 303,
		SAL_KEYCODE_F15 = 304,
		SAL_KEYCODE_F16 = 305,
		SAL_KEYCODE_F17 = 306,
		SAL_KEYCODE_F18 = 307,
		SAL_KEYCODE_F19 = 308,
		SAL_KEYCODE_F20 = 309,
		SAL_KEYCODE_F21 = 310,
		SAL_KEYCODE_F22 = 311,
		SAL_KEYCODE_F23 = 312,
		SAL_KEYCODE_F24 = 313,
		SAL_KEYCODE_F25 = 314,
		SAL_KEYCODE_KP_0 = 320,
		SAL_KEYCODE_KP_1 = 321,
		SAL_KEYCODE_KP_2 = 322,
		SAL_KEYCODE_KP_3 = 323,
		SAL_KEYCODE_KP_4 = 324,
		SAL_KEYCODE_KP_5 = 325,
		SAL_KEYCODE_KP_6 = 326,
		SAL_KEYCODE_KP_7 = 327,
		SAL_KEYCODE_KP_8 = 328,
		SAL_KEYCODE_KP_9 = 329,
		SAL_KEYCODE_KP_DECIMAL = 330,
		SAL_KEYCODE_KP_DIVIDE = 331,
		SAL_KEYCODE_KP_MULTIPLY = 332,
		SAL_KEYCODE_KP_SUBTRACT = 333,
		SAL_KEYCODE_KP_ADD = 334,
		SAL_KEYCODE_KP_ENTER = 335,
		SAL_KEYCODE_KP_EQUAL = 336,
		SAL_KEYCODE_LEFT_SHIFT = 340,
		SAL_KEYCODE_LEFT_CONTROL = 341,
		SAL_KEYCODE_LEFT_ALT = 342,
		SAL_KEYCODE_LEFT_SUPER = 343,
		SAL_KEYCODE_RIGHT_SHIFT = 344,
		SAL_KEYCODE_RIGHT_CONTROL = 345,
		SAL_KEYCODE_RIGHT_ALT = 346,
		SAL_KEYCODE_RIGHT_SUPER = 347,
		SAL_KEYCODE_MENU = 348,
	};

	struct Point2D
	{
		float x;
		float y;
	};

	struct WindowDetails
	{
		const char* title;
		int width;
		int height;
	};

	enum KeyMod : int
	{
		ModShift	= (1 << 0),
		ModCtrl		= (1 << 1),
		ModAlt		= (1 << 2),
		ModSuper	= (1 << 3)
	};

	///////////////////////////////////////////////////////////////////////////////////////////////

	struct Message
	{
		explicit Message() = default;
		explicit Message(uint16_t type) : m_type(type) {}

		uint16_t m_type = 0;
		uint16_t m_param1 = 0;
		uint32_t m_param2 = 0;
		std::unique_ptr<uint8_t[]> m_payload;
	};

	class MessageQueue
	{
		friend class App;

	public:
		void Add(Message e)
		{
			m_messageQueue.emplace_back(std::move(e));
		}
	private:
		std::deque<Message> m_messageQueue;
	};

	class App;
	class ViewIO;

	///////////////////////////////////////////////////////////////////////////////////////////////

	constexpr int kQuitStateId = 0;
	constexpr int kMasterStateId = 1;

	class State
	{
	public:
		State(App& app, State* parent) : m_app(app), m_parent(parent) {}
		virtual ~State() {}

		virtual void Init() {}

		virtual bool HandleMessage(Message& m)
		{
			return false;
		}

		virtual void Update(double elapsedTime) = 0;
		virtual int GetStateId() const = 0;

		virtual void Finish() {};

	protected:
		void ChangeState(int stateId);

	private:
		App& m_app;
		State* m_parent;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////

	template<typename ThisState, typename ParentState, int STATEID>
	class ConcreteState : public ParentState
	{
	public:
		static std::unique_ptr<State> Create(App& app, State* parent)
		{
			return std::unique_ptr<State>(new ThisState(app, parent));
		}

		static const int stateId = STATEID;
		virtual int GetStateId() const
		{
			return stateId;
		}

	protected:
		ConcreteState<ThisState, ParentState, STATEID>(App& app, State* parent)
			: ParentState(app, parent)
		{}

		typedef ConcreteState<ThisState, ParentState, STATEID> ParentClass;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////

	class StateRenderer
	{
	public:
		StateRenderer(ViewIO* viewIO, State* state, StateRenderer* parent)
			: m_viewIO(viewIO)
			, m_state(state)
			, m_parent(parent) {}

		virtual ~StateRenderer() {}

		virtual void Init() = 0;
		virtual void Focus() {}
		virtual void Render() = 0;

		State* GetState() const { return m_state; }

		virtual void OnKeyboardInput(bool down, int keyCode) {}
		virtual void OnKeyCharacter(int keyCode, int modifiers) {}
		virtual void OnMouseInput(bool down, int mouseButtonCode) {}
		virtual void OnMouseMove(const Point2D& mouseXY) {}

	protected:
		ViewIO* GetViewIO() const { return m_viewIO; }

	private:
		State* m_state;
		StateRenderer* m_parent;
		ViewIO* m_viewIO;
	};

	template <typename C>
	class ConcreteStateRenderer
	{
	public:
		static std::unique_ptr<StateRenderer> Create(ViewIO* viewIo, State* state, StateRenderer* parent)
		{
			return std::unique_ptr<StateRenderer>(new C(viewIo, state, parent));
		}
	};

	///////////////////////////////////////////////////////////////////////////////////////////////

	class MasterState : public ConcreteState<MasterState, State, kMasterStateId>
	{
	public:
		MasterState(App& app, State* parent) : ParentClass(app, parent) {}

		//virtual bool HandleMessage(Message* e) override;
		virtual void Update(double elapsedTime) override {}
	};

	typedef std::function<std::unique_ptr<State>(App&, State*)> CreateStateFunc;
	typedef std::function<std::unique_ptr<StateRenderer>(ViewIO*, State*, StateRenderer*)> CreateStateRendererFunc;

	///////////////////////////////////////////////////////////////////////////////////////////////

	class App
	{
		friend class AppDriver;
		friend class State;

	public:
		explicit App(std::unique_ptr<AppDriver>&& driver);

		bool Run(const sal::WindowDetails& details);

	private:
		void ChangeState(int stateId)
		{
			m_pendingStateChange = stateId;
		}

	private:
		virtual void OnStartup() = 0;
		//virtual void OnEvent() = 0;
		virtual void OnShutdown() = 0;

		void Init();
		bool Frame(double elapsedTime);
		void Render();

		void KeyboardInput(bool down, int keyCode);
		void KeyCharacter(int keyCode, int modifiers);
		void MouseInput(bool down, int mouseButtonCode);
		void MouseMove(const Point2D& mouseXY);

		void Cleanup();

		void PushState(int stateId);
		void PopState();
		void DoStateChange();
		bool IsChildState(int stateId, int childState) const;

		void HandleMessages();

	public:
		void RegisterState(int stateId, int parentStateId,
			CreateStateFunc createFunc, CreateStateRendererFunc createRendererFunc);

	private:

		int m_pendingStateChange = -1;
		std::vector<std::unique_ptr<State>> m_stateStack;
		std::vector<std::unique_ptr<StateRenderer>> m_stateRendererStack;

		struct RegisteredState
		{
			int initialChildStateId;
			int parentStateId;
			CreateStateFunc createFunc;
			CreateStateRendererFunc createRendererFunc;
		};
		std::vector<RegisteredState> m_stateTable;

		std::unique_ptr<AppDriver> m_driver;

		MessageQueue m_messages;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	////// API SIDE //////

	class ViewIO
	{
		friend class AppDriver;
	public:
		// Engine overrides
		virtual void Setup() = 0;
		virtual void OnRenderBegin() = 0;
		virtual void OnRenderEnd() = 0;
		virtual void Teardown() = 0;

		void AddMessage(Message m)
		{
			m_messages->Add(std::move(m));
		}

		// User overrides
		virtual void OnStartup() {};
		virtual void OnShutdown() {};

		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }
	private:

		void SetMessages(MessageQueue& mq)
		{
			m_messages = &mq;
		}

	private:
		MessageQueue* m_messages = nullptr;

		int m_width;
		int m_height;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////

	class AppDriver
	{
		friend class App;
	public:
		explicit AppDriver(std::unique_ptr<ViewIO> viewIO) : m_viewIO(std::move(viewIO)) { _ASSERT(bool(m_viewIO)); }
		virtual ~AppDriver() {}

	private:
		virtual bool Run(const WindowDetails& details) = 0;
		void SetApp(App* app)
		{
			m_app = app;
			m_viewIO->SetMessages(app->m_messages);
		}

	protected:
		void CallInit()
		{
			m_viewIO->Setup();
			m_viewIO->OnStartup();
			m_app->Init();
		}
		bool CallFrame(double elapsedTime) { return m_app->Frame(elapsedTime); }
		void CallRender()
		{
			m_viewIO->OnRenderBegin();
			m_app->Render();
			m_viewIO->OnRenderEnd();
		}

		void KeyboardInput(bool down, int keyCode)
		{
			_ASSERT(m_app);
			m_app->KeyboardInput(down, keyCode);
		}
		void KeyCharacter(int keyCode, int modifiers)
		{
			_ASSERT(m_app);
			m_app->KeyCharacter(keyCode, modifiers);
		}
		void MouseInput(bool down, int mouseButtonCode)
		{
			_ASSERT(m_app);
			m_app->MouseInput(down, mouseButtonCode);
		}
		void MouseMove(const Point2D& mouseXY)
		{
			_ASSERT(m_app);
			m_app->MouseMove(mouseXY);
		}

		void CallCleanup()
		{
			m_app->Cleanup();
			m_viewIO->OnShutdown();
			m_viewIO->Teardown();
			m_viewIO.reset(); // call ViewIO destructor here
		}

		void SetWidthHeight(int width, int height)
		{
			m_viewIO->m_width = width;
			m_viewIO->m_height = height;
		}

	private:
		App* m_app = nullptr;
	protected:
		std::unique_ptr<ViewIO> m_viewIO;
	};

	inline void State::ChangeState(int stateId)
	{
		m_app.ChangeState(stateId);
	}

} // namespace sal
