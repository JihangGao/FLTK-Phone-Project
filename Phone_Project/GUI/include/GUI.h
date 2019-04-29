
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#ifndef GUI_GUARD
#define GUI_GUARD

#include "Window.h"
#include "Graph.h"

namespace Graph_lib {

//------------------------------------------------------------------------------

    typedef void* Address;    // Address is a synonym for void*
    typedef void(*Callback)(Address, Address);    // FLTK's required function type for all callbacks

//------------------------------------------------------------------------------

    template<class W> W& reference_to(Address pw)
    // treat an address as a reference to a W
    {
        return *static_cast<W*>(pw);
    }

//------------------------------------------------------------------------------

    class Widget {
    // Widget is a handle to an Fl_widget - it is *not* an Fl_widget
    // We try to keep our interface classes at arm's length from FLTK
    public:
        Widget(Point xy, int w, int h, const string& s, Callback cb)
            : loc(xy), width(w), height(h), label(s), do_it(cb)
        {}

        virtual void move(int dx,int dy) { hide(); pw->position(loc.x+=dx, loc.y+=dy); show(); }
        virtual void hide() { pw->hide(); }
        virtual void show() { pw->show(); }
        virtual void attach(Window&) = 0;

        Point loc;
        int width;
        int height;
        string label;
        Callback do_it;

        virtual ~Widget() { }

    protected:
        Window* own;    // every Widget belongs to a Window
        Fl_Widget* pw;  // connection to the FLTK Widget
    private:
        Widget& operator=(const Widget&); // don't copy Widgets
        Widget(const Widget&);
    };

//------------------------------------------------------------------------------

    struct Button : Widget {
        Button(Point xy, int w, int h, const string& label, Callback cb)
            : Widget(xy,w,h,label,cb)
        {}

        void attach(Window&);
    };

//------------------------------------------------------------------------------

    struct In_box : Widget {
        In_box(Point xy, int w, int h, const string& s)
            :Widget(xy,w,h,s,0) { }
        int get_int();
        string get_string();

        void attach(Window& win);
    };

//------------------------------------------------------------------------------

    struct Out_box : Widget {
        Out_box(Point xy, int w, int h, const string& s)
            :Widget(xy,w,h,s,0) { }
        void put(int);
        void put(const string&);

        void attach(Window& win);
    };

//------------------------------------------------------------------------------

    struct Menu : Widget {
        enum Kind { horizontal, vertical };
        Menu(Point xy, int w, int h, Kind kk, const string& label)
            : Widget(xy,w,h,label,0), k(kk), offset(0)
        {}

        Vector_ref<Button> selection;
        Kind k;
        int offset;
        int attach(Button& b);      // Menu does not delete &b
        int attach(Button* p);      // Menu deletes p

        void show()                 // show all buttons
        {
            for (unsigned int i = 0; i<selection.size(); ++i)
                selection[i].show();
        }
        void hide()                 // hide all buttons
        {
            for (unsigned int i = 0; i<selection.size(); ++i) 
                selection[i].hide(); 
        }
        void move(int dx, int dy)   // move all buttons
        {
            for (unsigned int i = 0; i<selection.size(); ++i) 
                selection[i].move(dx,dy);
        }

        void attach(Window& win)    // attach all buttons
        {
            for (int i=0; i<selection.size(); ++i) win.attach(selection[i]);
            own = &win;
        }

    };
//------------------------------------------------------------------------------
	struct Random_Window : Window
	{
		Random_Window(Point xy, int w, int h, const string& title, string address_img);
	private:
		Button next_button;
		Button quit_button;
		void next();
		void quit();
		Out_box xy_out;
		static void cb_next(Address, Address);
		static void cb_quit(Address, Address);
		Image pic;
	};
//------------------------------------------------------------------------------

	struct Air_Window : Window
	{
		Air_Window(Point(xy), int w, int h, const string& title);
		int vv() { return v; };
	private:
		int v;
		Button stop_button;
		Button start_button;
		Button quit_button;
		void stop();
		void start();
		void quit();
		Out_box condition;
		static void cb_start(Address, Address);
		static void cb_stop(Address, Address);
		static void cb_quit(Address, Address);
	};
//------------------------------------------------------------------------------

	struct Cal_Window : Window
	{
		Cal_Window(Point(xy), int w, int h, const string& title);
	private:
		Button cal_button;
		Button quit_button;
		void cal();
		void quit();
		Out_box result;
		In_box exp;
		static void cb_cal(Address, Address);
		static void cb_quit(Address, Address);
	};

	template <typename Target, typename Source>
	Target narrow_cast(Source src)
	{
		Target tgt = src;

		if ((Source)tgt != src)
			error("Invalid narrowing conversion");

		return tgt;
	}
//------------------------------------------------------------------------------
	//PLUG FUNCTION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	struct Phone : Window
	{
		Phone();
	private:
		//Button up_button;
		//Button down_button;
		//Button home_button;
		Button Lock_button;
		//Button Mute_button;
		Image phone_fraction;
		Image lock_screen;
		Image closed_screen;
		//Image Info_box;
		//Out_box time_output;
		//void switchoff();
		//static void cb_switchoff(Address, Address);
		void switchon();
		static void cb_switchon(Address, Address);
		//void soundup();
		//static void cb_soundup(Address, Address);
		//void sounddn();
		//static void cb_sounddn(Address, Address);
		//void back_to_home();
		//static void cb_back_to_home(Address, Address);
		//void lock();
		//static void cb_lock(Address, Address);
		//void open();
		//static void cb_open(Address, Address);
		int screen_status;
		const bool off = true;
		const bool on = false;
	};

//------------------------------------------------------------------------------
} // of namespace Graph_lib

#endif // GUI_GUARD