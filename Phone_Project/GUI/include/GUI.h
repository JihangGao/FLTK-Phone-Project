
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#ifndef GUI_GUARD
#define GUI_GUARD

#include "Window.h"
#include "Graph.h"
#include <FL\Fl_Button.H>
#include <FL/Fl_Widget.H>
#include <iostream>
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

		virtual void move(int dx, int dy) { hide(); pw->position(loc.x += dx, loc.y += dy); show(); }
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
		Window * own;    // every Widget belongs to a Window
		Fl_Widget* pw;  // connection to the FLTK Widget
	private:
		Widget & operator=(const Widget&); // don't copy Widgets
		Widget(const Widget&);
	};

//------------------------------------------------------------------------------

    struct Button : Widget {
        Button(Point xy, int w, int h, const string& label, Callback cb)
            : Widget(xy,w,h,label,cb)
        {}

        void attach(Window&);
    };

	struct appButton : Widget {
		appButton(Point xy, int w, int h, const string& label, Callback cb)
			: Widget(xy, w, h, label, cb)
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
	class MyApp : public Fl_Button
	{
		
	public:
		void draw();
		MyApp(int x, int y, int w, int h, const char* l = 0)
			:Fl_Button(x, y, w, h, l)
		{}
		int handle(int e) {
			switch (e) {
				case FL_PUSH:
					cout << endl << "Button C callback!" << endl;
					show();
					do_callback();
					break;
				case FL_RELEASE:
					cout << endl << "Button A callback!" << endl;
					break;
				default: return 0;
			}
			
			return(Fl_Button::handle(e));

		};
	};

//------------------------------------------------------------------------------
	//PLUG FUNCTION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	struct Phone : Window
	{
		Phone();
	private:
		appButton pass_1;
		appButton pass_2;
		appButton pass_3;
		appButton pass_4;
		appButton pass_5;
		appButton pass_6;
		appButton pass_7;
		appButton pass_8;
		appButton pass_9;
		appButton pass_0;
		Button passdelete;
		Button Up_button;
		Button Down_button;
		//Button home_button;
		Button Lock_button;
		Button Mute_button;
		Button home_button;
		Image Password;
		Image p0;
		Image p1;
		Image p2;
		Image p3;
		Image p4;
		Image p5;
		Image p6;
		Image p7;
		Image p8;
		Image p9;
		Image Phone_fraction;
		Image Lock_screen;
		Image Closed_screen;
		Image Starting_screen;
		Image Desktop;
		Image Sound_screen_0;
		Image Sound_screen_1;
		Image Sound_screen_2;
		Image Sound_screen_3;
		Image Sound_screen_4;
		Image Sound_screen_5;
		Image Sound_screen_6;
		Image Sound_screen_7;
		Image Sound_screen_8;
		Image Sound_screen_9;
		Image Sound_screen_10;
		Image Sound_screen_11;
		Image Sound_screen_12;
		Image Sound_screen_13;
		Image Sound_screen_14;
		Image Sound_screen_15;
		Image Sound_screen_16;
		Image pass_titlewrong;
		Out_box Current_Screen_state;
		Out_box Current_Sound_state;
		Out_box password_state;
		Image pass_title0;
		Image pass_title1;
		Image pass_title2;
		Image pass_title3;
		Image pass_title4;
		//Image Info_box;
		//Out_box time_output;
		Image& show_sound(int);
		void home();
		static void cb_home(Address, Address);
		void switchon();
		//static void cb_home(Fl_Widget*, void*);
		static void cb_switchon(Address, Address);
		void soundup();
		static void cb_soundup(Address, Address);
		void sounddn();
		static void cb_sounddn(Address, Address);
		void mute();
		static void cb_mute(Address, Address);
		Image& pass_title(int i) {
			switch (i) {
			case 0: return pass_title0;
			case 1: return pass_title1;
			case 2: return pass_title2;
			case 3: return pass_title3;
			case 4: return pass_title4;
			}
		}
		void pass1();
		void pass2();
		void pass3();
		void pass4();
		void pass5();
		void pass6();
		void pass7();
		void pass8();
		void pass9();
		void pass0();
		void pass_delete();
		void unlock();
		void detach_unlock(int);
		void attach_unlock();
		static void cb_pass_delete(Address, Address);
		static void cb_pass1(Address, Address);
		static void cb_pass2(Address, Address);
		static void cb_pass3(Address, Address);
		static void cb_pass4(Address, Address);
		static void cb_pass5(Address, Address);
		static void cb_pass6(Address, Address);
		static void cb_pass7(Address, Address);
		static void cb_pass8(Address, Address);
		static void cb_pass9(Address, Address);
		static void cb_pass0(Address, Address);
		//void back_to_home();
		//static void cb_back_to_home(Address, Address); 
		int Sound_status;
		int Sound_saved;
		int Screen_status;
		int lock_condition[10] = {1,1,1,1,1,1,1,1,1,1};
		int lock_count = 0;
		int password[4] = { 0 };
		int pass_word[4] = { 1,2,3,4 };
		stringstream sss;
	};

//------------------------------------------------------------------------------
} // of namespace Graph_lib

#endif // GUI_GUARD
