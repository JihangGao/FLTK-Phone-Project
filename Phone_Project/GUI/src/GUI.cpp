
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//
#pragma warning(disable : 4996)
#include "GUI.h"
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Output.H>
#include <random>
#include <Windows.h>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#define poweroff 0
#define starting 1
#define closed 2
#define locked 3
#define desktop 4
#define inapp 5
#define passwording 6
#define muted 0
namespace Graph_lib {

//------------------------------------------------------------------------------
	drag_delta contact_delta{ 0,0,0,0,0 };
void Button::attach(Window& win)
{
	pw = new Fl_Button(loc.x, loc.y, width, height, label.c_str());
	pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
	own = &win;
}

void appButton::attach(Window& win)
{
	pw = new MyApp(loc.x, loc.y, width, height, label.c_str());
	pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
	own = &win;
}

void contact_bar::attach(Window& win)
{
	pw = new contactbar(loc.x, loc.y, width, height, label.c_str());
	pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
	own = &win;
}

void powerButton::attach(Window& win)
{
	pw = new power(loc.x, loc.y, width, height, label.c_str());
	pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
	own = &win;
}

void timeButton::attach(Window& win)
{
	pw = new time_button(loc.x, loc.y, width, height, label.c_str());
	pw->callback(reinterpret_cast<Fl_Callback*>(do_it), &win); // pass the window
	own = &win;
}
//------------------------------------------------------------------------------

int In_box::get_int()
{
    Fl_Input& pi = reference_to<Fl_Input>(pw);
    // return atoi(pi.value());
    const char* p = pi.value();
    if (!isdigit(p[0])) return -999999;
    return atoi(p);
}

string In_box::get_string()
{
	Fl_Input& pi = reference_to<Fl_Input>(pw);
	// return atoi(pi.value());
	const char* p = pi.value();
	return p;
}

//------------------------------------------------------------------------------

void In_box::attach(Window& win)
{
    pw = new Fl_Input(loc.x, loc.y, width, height, label.c_str());
    own = &win;
}

//------------------------------------------------------------------------------

void Out_box::put(const string& s)
{
    reference_to<Fl_Output>(pw).value(s.c_str());
}

//------------------------------------------------------------------------------

void Out_box::attach(Window& win)
{
    pw = new Fl_Output(loc.x, loc.y, width, height, label.c_str());
    own = &win;
}

//------------------------------------------------------------------------------

int Menu::attach(Button& b)
{
    b.width = width;
    b.height = height;

    switch(k) {
    case horizontal:
        b.loc = Point(loc.x+offset,loc.y);
        offset+=b.width;
        break;
    case vertical:
        b.loc = Point(loc.x,loc.y+offset);
        offset+=b.height;
        break;
    }
    selection.push_back(b); // b is NOT OWNED: pass by reference
    return int(selection.size()-1);
}

//------------------------------------------------------------------------------

int Menu::attach(Button* p)
{
    Button& b = *p;
    b.width = width;
    b.height = height;

    switch(k) {
    case horizontal:
        b.loc = Point(loc.x+offset,loc.y);
        offset+=b.width;
        break;
    case vertical:
        b.loc = Point(loc.x,loc.y+offset);
        offset+=b.height;
        break;
    }
    selection.push_back(&b); // b is OWNED: pass by pointer
    return int(selection.size()-1);
}

//------------------------------------------------------------------------------

Random_Window::Random_Window(Point xy, int w, int h, const string& title, string address_img)
	:Window(xy, w, h, title),
	pic(Point(x_max() - 150, 0), address_img),
	next_button(Point(x_max() - 150, 0), 70, 20, "Next point", cb_next),
	quit_button(Point(x_max() - 70, 0), 70, 20, "Quit", cb_quit),
	xy_out(Point(100, 0), 100, 20, "current (x,y):")
{
	pic.set_img(Point(x_max() - 150, 0), 70, 20);
	attach(next_button);
	attach(quit_button);
	attach(xy_out);
	attach(pic);	
}

void Random_Window::cb_quit(Address, Address pw)
{
	reference_to<Random_Window>(pw).quit();
}

void Random_Window::quit()
{
	hide();        // curious FLTK idiom for delete window
}

inline int rand_int(int min, int max)
{
	static default_random_engine ran;
	return uniform_int_distribution<>(min, max)(ran);
}

void Random_Window::cb_next(Address, Address pw)     // "the usual"
{
	reference_to<Random_Window>(pw).next();
}

void Random_Window::next()
{
	next_button.hide();
	next_button.loc.x = rand_int(0, x_max());
	next_button.loc.y = rand_int(0, y_max());
	pic.set_img(Point(next_button.loc.x, next_button.loc.y), 70, 20);
	attach(pic);
	attach(next_button);
	stringstream ss;
	ss << '(' << next_button.loc.x << ',' << next_button.loc.y << ')';
	xy_out.put(ss.str());
	redraw();
}

//------------------------------------------------------------------------------

Air_Window::Air_Window(Point xy, int w, int h, const string& title)
	:Window(xy, w, h, title),
	v(0),
	start_button(Point(x_max() - 150, 0), 70, 20, "Start", cb_start),
	quit_button(Point(x_max() - 70, 0), 70, 20, "Quit", cb_quit),
	stop_button(Point(x_max() - 230, 0), 70, 20, "Stop", cb_stop),
	condition(Point(100, 0), 100, 20, "current situation:")
{
	attach(start_button);
	attach(quit_button);
	attach(stop_button);
	attach(condition);
}

void Air_Window::cb_quit(Address, Address pw)
{
	reference_to<Air_Window>(pw).quit();
}

void Air_Window::quit()
{
	hide();        // curious FLTK idiom for delete window
}

void Air_Window::cb_start(Address, Address pw)     // "the usual"
{
	reference_to<Air_Window>(pw).start();
}

void Air_Window::start()
{
	v = 2;
	//detach(condition);
	stringstream ss;
	ss << "MOVING";
	condition.put(ss.str());
	redraw();
	
}

void Air_Window::cb_stop(Address, Address pw)     // "the usual"
{
	reference_to<Air_Window>(pw).stop();
}

void Air_Window::stop()
{
	v = 0;
	stringstream ss;
	ss << "STOP";
	condition.put(ss.str());
	redraw();
}
//------------------------------------------------------------------------------

Cal_Window::Cal_Window(Point xy, int w, int h, const string& title)
	:Window(xy, w, h, title),
	quit_button(Point(x_max() - 70, 0), 70, 20, "Quit", cb_quit),
	cal_button(Point(200, 200), 100, 100, "Calculate", cb_cal),
	result(Point(100, 150), 100, 20, "Result:"),
	exp(Point(100, 100), 300, 20, "Expression:")
{
	attach(quit_button);
	attach(cal_button);
	attach(result);
	attach(exp);
}

void Cal_Window::cb_quit(Address, Address pw)
{
	reference_to<Cal_Window>(pw).quit();
}

void Cal_Window::quit()
{
	hide();        // curious FLTK idiom for delete window
}

void Cal_Window::cb_cal(Address, Address pw)
{
	reference_to<Cal_Window>(pw).cal();
}

int pos_current = 0;
string expr_storage;
const char number = '8';    // t.kind==number means that t is a number Token
const char quit = 'q';    // t.kind==quit means that t is a quit Token
const char print = '\n';    // t.kind==print means that t is a print Token
bool variate_or_not = false;
bool erase_output = false;
const string prompt = "> ";
const string result = ""; // used to indicate that what follows is a result
int count_of_v = 2;	//------------------------------------------------------------------------------
struct Name {
	string name;
	double value;
}variate[1000] = { { "pi",3.1415926535 },{ "e",2.7182818284 } };

//3.1415926535   2.7182818284
class Token {
public:
	char kind;        // what kind of token
	double value;     // for numbers: a value 
	Token(char ch)    // make a Token from a char
		:kind(ch), value(0) { }
	Token(char ch, double val)     // make a Token from a char and a double
		:kind(ch), value(val) { }
	static void Varify();
	static Token check_variate(char ch);
};
void Token::Varify() {
	char bin;
	getchar();
	getchar();
	getchar();//come to variate
	cin >> variate[count_of_v].name;
	cin >> bin;//come to calculate result
	variate_or_not = true;
	return;
}
Token Token::check_variate(char ch) {
	char temp_var[1000];
	string tempenter;
	temp_var[0] = ch;
	for (int i = 1;; i++) {
		temp_var[i] = getchar();
		if (temp_var[i] == '+' || temp_var[i] == '-' || temp_var[i] == '*' || temp_var[i] == '/' || temp_var[i] == ' ' || temp_var[i] == ')' || temp_var[i] == '\n') {
			cin.putback(temp_var[i]);
			cin.putback(' ');
			for (int j = i - 1; j >= 0; j--) cin.putback(temp_var[j]);
			cin >> tempenter;
			for (int j = 0; j < count_of_v; j++) {
				if (variate[j].name == tempenter) {
					return Token('8', variate[j].value);
				}
			}
		}
	}
}
//------------------------------------------------------------------------------

class Token_stream {
public:
	Token_stream();   // make a Token_stream that reads from cin
	Token get();      // get a Token (get() is defined elsewhere)
	void putback(Token t);    // put a Token back
	void ignore(char c); // discard tokens up to an including a c
	void initialize() { full = false; };
private:
	bool full;        // is there a Token in the buffer?
	Token buffer;     // here is where we keep a Token put back using putback()
};

//------------------------------------------------------------------------------

// The constructor just sets full to indicate that the buffer is empty:
Token_stream::Token_stream()
	:full(false), buffer(0)    // no Token in buffer
{
}

//------------------------------------------------------------------------------

// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
	if (full) error("putback() into a full buffer");
	buffer = t;       // copy t to buffer
	full = true;      // buffer is now full
}

//------------------------------------------------------------------------------

Token Token_stream::get() // read characters from cin and compose a Token
{
	if (full) {         // check if we already have a Token ready
		full = false;
		return buffer;
	}
	for (;;) {
		char ch;
		for (;;) {
			ch = expr_storage[pos_current];
			if (ch != ' ') {
				pos_current++;
				break;
			}
		}// note that >> skips whitespace (space, newline, tab, etc.)

		switch (ch) {
		case quit:
		case '(':
		case print:
		case ';':
		case ')':
		case '}':
		case '{':
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
			return Token(ch); // let each character represent itself
		case '.':             // a floating-point literal can start with a dot
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':    // numeric literal
		{
			int flag_point;
			int flag_end = expr_storage.length() - 1;
			for (int j = pos_current;; j++) {
				if (expr_storage[j] < '0' || expr_storage[j] > '9') {
					if (expr_storage[j] == '.') {
						flag_point = j;
						for (int t = j + 1; t <= flag_end; t++) {
							if (expr_storage[t] < '0' || expr_storage[t] > '9') {
								flag_end = t - 1;
								break;
							}
						}
						break;
					}
					else {
						flag_end = j - 1;
						flag_point = -1;
						break;
					}
				}
			}
			string strval = "";
			for (int i = pos_current - 1; i <= flag_end; i++) {
				strval += expr_storage[i];
			}
			pos_current = flag_end + 1;
			double val = stod(strval);     // read a floating-point number
			return Token(number, val);
		}
		case 'l': Token::Varify(); break;
		default: return Token::check_variate(ch);
		}
		if (variate_or_not) {
			variate_or_not = false;
			erase_output = true;
		}
		else break;
	}
}

//------------------------------------------------------------------------------

void Token_stream::ignore(char c)
// c represents the kind of a Token
{
	// first look in buffer:
	if (full && c == buffer.kind) {
		full = false;
		return;
	}
	full = false;

	// now seach input:
	char ch = 0;
	while (cin >> ch)
		if (ch == c) return;
}

//------------------------------------------------------------------------------

Token_stream ts;        // provides get() and putback() 

						//------------------------------------------------------------------------------

double expression();    // declaration so that primary() can call expression()

						//------------------------------------------------------------------------------

						// deal with numbers and parentheses
double primary()
{
	Token t = ts.get();
	switch (t.kind) {
	case '(':           // handle '(' expression ')'
	{
		double d = expression();
		t = ts.get();
		if (t.kind != ')') error("')' expected");
		return d;
	}
	case '{':           // handle '(' expression ')'
	{
		double d = expression();
		t = ts.get();
		if (t.kind != '}') error("'}' expected");
		return d;
	}
	case number:
		return t.value;    // return the number's value
	case '-':
		return -primary();
	case '+':
		return primary();
	default:
		error("primary expected");
	}
}

//------------------------------------------------------------------------------

// deal with *, /, and %
double term()
{
	double left = primary();
	Token t = ts.get(); // get the next token from token stream

	while (true) {
		switch (t.kind) {
		case '*':
			left *= primary();
			t = ts.get();
			break;
		case '/':
		{
			double d = primary();
			if (d == 0) error("divide by zero");
			left /= d;
			t = ts.get();
			break;
		}
		case '%':
		{
			int i1 = narrow_cast<int>(left);
			int i2 = narrow_cast<int>(term());
			if (i2 == 0) error("%: divide by zero");
			left = i1 % i2;
			t = ts.get();
			break;
		}
		default:
			ts.putback(t);        // put t back into the token stream
			return left;
		}
	}
}

//------------------------------------------------------------------------------

// deal with + and -
double expression()
{
	double left = term();      // read and evaluate a Term
	Token t = ts.get();        // get the next token from token stream

	while (true) {
		switch (t.kind) {
		case '+':
			left += term();    // evaluate Term and add
			t = ts.get();
			break;
		case '-':
			left -= term();    // evaluate Term and subtract
			t = ts.get();
			break;
		default:
			ts.putback(t);     // put t back into the token stream
			return left;       // finally: no more + or -: return the answer
		}
	}
}

//------------------------------------------------------------------------------

void clean_up_mess()
{
	ts.ignore(print);
}

//------------------------------------------------------------------------------

double calculate()
{
	//while (cin)
		try {
		Token t = ts.get();
		  // first discard all "prints"
		//if (t.kind == quit) return;            // quit
		ts.putback(t);
		if (erase_output) {
			variate[count_of_v].value = expression();
			count_of_v++;
			erase_output = false;
			variate_or_not = false;
		}
		return expression();
	}
	catch (runtime_error& e) {
		//cerr << e.what() << endl;              // write error message
		clean_up_mess();
	}
}

void Cal_Window::cal()
{
	ts.initialize();
	pos_current = 0;
	expr_storage = "\0";
	string obj = exp.get_string() + ";";
	expr_storage = obj;
	stringstream ss;
	ss << calculate();
	result.put(ss.str());
	redraw();
}
//------------------------------------------------------------------------------
void MyApp::draw()
{
	if (type() == FL_HIDDEN_BUTTON) return;
	Fl_Color col = value() ? selection_color() : color();
	draw_box(value() ? (down_box() ? down_box() : fl_down(box())) : box(), col);
	draw_backdrop();
	if (labeltype() == FL_NORMAL_LABEL && value()) {
		Fl_Color c = labelcolor();
		labelcolor(fl_contrast(c, col));
		draw_label();
		labelcolor(c);
	}
	else draw_label();
	if (Fl::focus() == this) draw_focus();
}
void power::draw() {
	if (type() == FL_HIDDEN_BUTTON) return;
	Fl_Color col = value() ? selection_color() : color();
	draw_box(value() ? (down_box() ? down_box() : fl_down(box())) : box(), col);
	draw_backdrop();
	if (labeltype() == FL_NORMAL_LABEL && value()) {
		Fl_Color c = labelcolor();
		labelcolor(fl_contrast(c, col));
		draw_label();
		labelcolor(c);
	}
	else draw_label();
	if (Fl::focus() == this) draw_focus();
}
void contactbar::draw() {
	if (type() == FL_HIDDEN_BUTTON) return;
	Fl_Color col = value() ? selection_color() : color();
	draw_box(value() ? (down_box() ? down_box() : fl_down(box())) : box(), col);
	draw_backdrop();
	if (labeltype() == FL_NORMAL_LABEL && value()) {
		Fl_Color c = labelcolor();
		labelcolor(fl_contrast(c, col));
		draw_label();
		labelcolor(c);
	}
	else draw_label();
	if (Fl::focus() == this) draw_focus();
}
int contactbar::handle(int e) {
	switch (e) {
	case FL_PUSH:
		contact_delta.prev_x = Fl::event_x();
		contact_delta.prev_y = Fl::event_y();
	case FL_DRAG:
		contact_delta.count++;
		if (contact_delta.count > 10) {
			contact_delta.count = 0;
			contact_delta.delta_x = Fl::event_x();
			contact_delta.delta_y = Fl::event_y();
			do_callback();
			contact_delta.prev_x = Fl::event_x();
			contact_delta.prev_y = Fl::event_y();
		}
		return 1;
	default: return 0;
	}
	return (Fl_Button::handle(e));
}
;
//------------------------------------------------------------------------------
Phone::Phone() :
	Window(Point(0, 0), 600, 960, "Phone"),
	Phone_fraction(Point(0, 0), "D:\\C++\\Phone_Project\\images\\fraction_bg.jpg"),
	//home_button(Point(209, 520), 10, 10, "", cb_back_to_home)
	Lock_button(Point(318, 120), 9, 45, "", cb_switchon),
	Lock_screen(Point(27, 75), "D:\\C++\\Phone_Project\\images\\lock.jpg"),
	Closed_screen(Point(27, 75), "D:\\C++\\Phone_Project\\images\\closed_bg.jpg"),
	Starting_p0(Point(27, 75), "D:\\C++\\Phone_Project\\images\\starting_p0.jpg"),
	Starting_p1(Point(27, 75), "D:\\C++\\Phone_Project\\images\\starting_p1.jpg"),
	Starting_p2(Point(27, 75), "D:\\C++\\Phone_Project\\images\\starting_p2.jpg"),
	Starting_p3(Point(27, 75), "D:\\C++\\Phone_Project\\images\\starting_p3.jpg"),
	Starting_screen(Point(27, 75), "D:\\C++\\Phone_Project\\images\\starting.jpg"),
	Up_button(Point(4, 121), 10, 41, "", cb_soundup),
	Down_button(Point(4, 174), 10, 41, "", cb_sounddn),
	Mute_button(Point(5, 74), 8, 32, "", cb_mute),
	Current_Screen_state(Point(200, 700), 100, 40, "Current Screen State:"),
	Current_Sound_state(Point(200, 800), 100, 40, "Current Sound State:"),
	Sound_screen_0(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s0.jpg"),
	Sound_screen_1(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s1.jpg"),
	Sound_screen_2(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s2.jpg"),
	Sound_screen_3(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s3.jpg"),
	Sound_screen_4(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s4.jpg"),
	Sound_screen_5(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s5.jpg"),
	Sound_screen_6(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s6.jpg"),
	Sound_screen_7(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s7.jpg"),
	Sound_screen_8(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s8.jpg"),
	Sound_screen_9(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s9.jpg"),
	Sound_screen_10(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s10.jpg"),
	Sound_screen_11(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s11.jpg"),
	Sound_screen_12(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s12.jpg"),
	Sound_screen_13(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s13.jpg"),
	Sound_screen_14(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s14.jpg"),
	Sound_screen_15(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s15.jpg"),
	Sound_screen_16(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s16.jpg"),
	p1(Point(61, 226), "D:\\C++\\Phone_Project\\images\\p1.jpg"),
	pass_1(Point(61, 226), 56, 56, "", cb_pass1),
	p2(Point(137, 226), "D:\\C++\\Phone_Project\\images\\p2.jpg"),
	pass_2(Point(137, 226), 56, 56, "", cb_pass2),
	p3(Point(213, 226), "D:\\C++\\Phone_Project\\images\\p3.jpg"),
	pass_3(Point(213, 226), 56, 56, "", cb_pass3),
	p4(Point(61, 293), "D:\\C++\\Phone_Project\\images\\p4.jpg"),
	pass_4(Point(61, 293), 56, 56, "", cb_pass4),
	p5(Point(137, 293), "D:\\C++\\Phone_Project\\images\\p5.jpg"),
	pass_5(Point(137, 293), 56, 56, "", cb_pass5),
	p6(Point(213, 293), "D:\\C++\\Phone_Project\\images\\p6.jpg"),
	pass_6(Point(213, 293), 56, 56, "", cb_pass6),
	p7(Point(61, 359), "D:\\C++\\Phone_Project\\images\\p7.jpg"),
	pass_7(Point(61, 359), 56, 56, "", cb_pass7),
	p8(Point(137, 359), "D:\\C++\\Phone_Project\\images\\p8.jpg"),
	pass_8(Point(137, 359), 56, 56, "", cb_pass8),
	p9(Point(213, 359), "D:\\C++\\Phone_Project\\images\\p9.jpg"),
	pass_9(Point(213, 359), 56, 56, "", cb_pass9),
	p0(Point(137, 425), "D:\\C++\\Phone_Project\\images\\p0.jpg"),
	pass_0(Point(137, 425), 56, 56, "", cb_pass0),
	Password(Point(27, 75), "D:\\C++\\Phone_Project\\images\\password.jpg"),
	pass_title1(Point(27, 135), "D:\\C++\\Phone_Project\\images\\pass_title1.jpg"),
	pass_title2(Point(27, 135), "D:\\C++\\Phone_Project\\images\\pass_title2.jpg"),
	pass_title3(Point(27, 135), "D:\\C++\\Phone_Project\\images\\pass_title3.jpg"),
	pass_title4(Point(27, 135), "D:\\C++\\Phone_Project\\images\\pass_title4.jpg"),
	pass_title0(Point(27, 135), "D:\\C++\\Phone_Project\\images\\pass_title0.jpg"),
	pass_titlewrong(Point(27, 135), "D:\\C++\\Phone_Project\\images\\pass_titlewrong.jpg"),
	Desktop(Point(27, 75), "D:\\C++\\Phone_Project\\images\\desktop.jpg"),
	home_button(Point(139, 578), 50, 50, "home", cb_home),
	//home_button(Point(500, 200), 50, 50, "home", cb_home),
	password_state(Point(200, 900), 100, 40, "password"),
	passdelete(Point(220, 527), 42, 18, "", cb_pass_delete),
	time_display(Point(101, 195)),
	time_display_2(Point(150, 90)),
	pro__0(Point(119 - 1, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_0(Point(119 + 0, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_1(Point(119 + 1, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_2(Point(119 + 2, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_3(Point(119 + 3, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_4(Point(119 + 4, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_5(Point(119 + 5, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_6(Point(119 + 6, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_7(Point(119 + 7, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_8(Point(119 + 8, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_9(Point(119 + 9, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_10(Point(119 + 10, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_11(Point(119 + 11, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_12(Point(119 + 12, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_13(Point(119 + 13, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_14(Point(119 + 14, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_15(Point(119 + 15, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_16(Point(119 + 16, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_17(Point(119 + 17, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_18(Point(119 + 18, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_19(Point(119 + 19, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_20(Point(119 + 20, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_21(Point(119 + 21, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_22(Point(119 + 22, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_23(Point(119 + 23, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_24(Point(119 + 24, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_25(Point(119 + 25, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_26(Point(119 + 26, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_27(Point(119 + 27, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_28(Point(119 + 28, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_29(Point(119 + 29, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_30(Point(119 + 30, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_31(Point(119 + 31, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_32(Point(119 + 32, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_33(Point(119 + 33, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_34(Point(119 + 34, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_35(Point(119 + 35, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_36(Point(119 + 36, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_37(Point(119 + 37, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_38(Point(119 + 38, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_39(Point(119 + 39, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_40(Point(119 + 40, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_41(Point(119 + 41, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_42(Point(119 + 42, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_43(Point(119 + 43, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_44(Point(119 + 44, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_45(Point(119 + 45, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_46(Point(119 + 46, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_47(Point(119 + 47, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_48(Point(119 + 48, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_49(Point(119 + 49, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_50(Point(119 + 50, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_51(Point(119 + 51, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_52(Point(119 + 52, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_53(Point(119 + 53, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_54(Point(119 + 54, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_55(Point(119 + 55, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_56(Point(119 + 56, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_57(Point(119 + 57, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_58(Point(119 + 58, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_59(Point(119 + 59, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_60(Point(119 + 60, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_61(Point(119 + 61, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_62(Point(119 + 62, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_63(Point(119 + 63, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_64(Point(119 + 64, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_65(Point(119 + 65, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_66(Point(119 + 66, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_67(Point(119 + 67, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_68(Point(119 + 68, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_69(Point(119 + 69, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_70(Point(119 + 70, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_71(Point(119 + 71, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_72(Point(119 + 72, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_73(Point(119 + 73, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_74(Point(119 + 74, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_75(Point(119 + 75, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_76(Point(119 + 76, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_77(Point(119 + 77, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_78(Point(119 + 78, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_79(Point(119 + 79, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_80(Point(119 + 80, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_81(Point(119 + 81, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_82(Point(119 + 82, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_83(Point(119 + 83, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_84(Point(119 + 84, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_85(Point(119 + 85, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_86(Point(119 + 86, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_87(Point(119 + 87, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_88(Point(119 + 88, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_89(Point(119 + 89, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_90(Point(119 + 90, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_91(Point(119 + 91, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_92(Point(119 + 92, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_93(Point(119 + 93, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_94(Point(119 + 94, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_95(Point(119 + 95, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_96(Point(119 + 96, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_97(Point(119 + 97, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_98(Point(119 + 98, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_99(Point(119 + 99, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_100(Point(119 + 100, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_101(Point(119 + 101, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_102(Point(119 + 102, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_103(Point(119 + 103, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_104(Point(119 + 104, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_105(Point(119 + 105, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_106(Point(119 + 106, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_107(Point(119 + 107, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_108(Point(119 + 108, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_109(Point(119 + 109, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_110(Point(119 + 110, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	pro_111(Point(119 + 111, 363), "D:\\C++\\Phone_Project\\images\\pro_1.jpg"),
	Contact(Point(27, 75), "D:\\C++\\Phone_Project\\images\\contact.jpg"),
	call_contact(Point(54, 512), 46, 46, "", cb_contact),
	name_0(Point(114, 280), "Bruno Bucciarati"),
	name_1(Point(114, 328), "Dio Brando"),
	name_2(Point(114, 376), "Guido Mista"),
	name_3(Point(114, 376 + 48), "Higashikata Josuke"),
	name_4(Point(114, 376 + 2 * 48), "King Crimson"),
	name_5(Point(114, 376 + 3 * 48), "Kujo Jotaro"),
	name_6(Point(114, 376 + 4 * 48), "Risotto Nero"),
	name_7(Point(114, 376 + 5 * 48), "Trish Una"),
	name_8(Point(114, 376 + 6 * 48), "Zeppeli Caesar"),
	name_9(Point(114, 376 + 7 * 48), "Zoootopia"),
	pg_up(Point(186+27, 125+75), 42, 32, "", cb_up),//down
	pg_dw(Point(132+27, 125+75), 42, 32, "", cb_dw),//up
	draging(Point(27, 165+75), 250, 294, "drag", cb_drag),
	search_name(Point(400, 200), 70, 20, "search:"),
	contact_detail(Point(27, 75), "D:\\C++\\Phone_Project\\images\\contact_detail.jpg"),
	name_show(Point(125, 205), ""),
	return_contact(Point(31, 97), 70, 20, "", cb_re_contact),
	detail_0(Point(279, 265), 17, 17, "", cb_call_detail_0),
	detail_1(Point(279, 265 + 48), 17, 17, "", cb_call_detail_1),
	detail_2(Point(279, 265 + 2 * 48), 17, 17, "", cb_call_detail_2),
	detail_3(Point(279, 265 + 3 * 48), 17, 17, "", cb_call_detail_3),
	detail_4(Point(279, 265 + 4 * 48), 17, 17, "", cb_call_detail_4),
	detail_5(Point(279, 265 + 5 * 48), 17, 17, "", cb_call_detail_5),
	do_search(Point(400, 300), 50, 50, "Search!", cb_dosearch),
	undo_search(Point(470, 300), 50, 50, "clear!", cb_undosearch)
{
	for (int i = 0; i <= 9; i++) {
		name(i).set_color(Color::black);
		name(i).set_font(Font::helvetica);
		name(i).set_font_size(12);
	}
	name_show.set_color(Color::black);
	name_show.set_font(Font::helvetica);
	name_show.set_font_size(15);
	attach(Lock_button);
	attach(Up_button);
	attach(Down_button);
	attach(Mute_button);
	attach(Phone_fraction);
	attach(Closed_screen);
	attach(Current_Screen_state);
	attach(Current_Sound_state);
	attach(home_button);
	attach(password_state);
	//home_button.callback(cb_home);
	Current_Screen_state.put("Power off");
	//attach(open_screen);
	Sound_saved = 8;
	Screen_status = poweroff;
	Sound_status = 8;
	lock_count = 0;
	power_off_try = 1;
};
void Phone::cb_dosearch(Address, Address pw) {
	reference_to<Phone>(pw).dosearch();
}
void Phone::dosearch() {
	PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\lock.wav"), NULL, SND_FILENAME | SND_ASYNC);
	string t = search_name.get_string();
	detach(pg_up);
	detach(pg_dw);
	detach(draging);
	searching(t);
}
void Phone::cb_undosearch(Address, Address pw) {
	reference_to<Phone>(pw).undosearch();
}
void Phone::undosearch() {
	PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\lock.wav"), NULL, SND_FILENAME | SND_ASYNC);
	contact_ini();
	attach(pg_up);
	attach(pg_dw);
	attach(draging);
	for (int i = 0; i <= 9; i++) {
		detach(name(i));
		if (name(i).point(0).y >= 280 && name(i).point(0).y <= 376 + 3 * 48) attach(name(i));
	}
	redraw();
}
void Phone::cb_re_contact(Address, Address pw) {
	reference_to<Phone>(pw).re_contact();
}
void Phone::re_contact(){
	PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\lock.wav"), NULL, SND_FILENAME | SND_ASYNC);
	detach(return_contact);
	detach(contact_detail);
	detach(name_show);
	for (int i = 0; i <= 5; i++) {
		attach(cont(i));
	}
	attach(do_search);
	attach(undo_search);
	attach(draging);
	attach(pg_up);
	attach(pg_dw);
	attach(Contact);
	Current_Screen_state.put("Viewing...");
	attach(search_name);
	for (int i = 0; i <= 9; i++) {
		detach(name(i));
		if (name(i).point(0).y >= 280 && name(i).point(0).y <= 376 + 3 * 48) attach(name(i));
	}
	redraw();
}
void Phone::call_detail(int num) {
	PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\lock.wav"), NULL, SND_FILENAME | SND_ASYNC);
	int t = find_name_detail(num);
	if (t == -1) return;
	detach_contact();
	attach(return_contact);
	attach(contact_detail);
	name_show.set_label(name(t).label());
	attach(name_show);
	redraw();
}
void Phone::cb_call_detail_0(Address, Address pw)
{
	reference_to<Phone>(pw).call_detail(0);
}
void Phone::cb_call_detail_1(Address, Address pw)
{
	reference_to<Phone>(pw).call_detail(1);
}
void Phone::cb_call_detail_2(Address, Address pw)
{
	reference_to<Phone>(pw).call_detail(2);
}
void Phone::cb_call_detail_3(Address, Address pw)
{
	reference_to<Phone>(pw).call_detail(3);
}
void Phone::cb_call_detail_4(Address, Address pw)
{
	reference_to<Phone>(pw).call_detail(4);
}
void Phone::cb_call_detail_5(Address, Address pw)
{
	reference_to<Phone>(pw).call_detail(5);
}
void Phone::cb_drag(Address, Address pw) {
	reference_to<Phone>(pw).drag();
}
void Phone::drag(){
	int alter = contact_delta.delta_y - contact_delta.prev_y;
	cout << -alter << endl;
	if (alter > 10) {
		dw();
	}
	else if (alter < -10) {
		up();
	}


}
void Phone::cb_up(Address, Address pw) {
	reference_to<Phone>(pw).up();
}
void Phone::up() {
	PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\lock.wav"), NULL, SND_FILENAME | SND_ASYNC);
	int curr_delta = -48;
	/*
	int alter = contact_delta.delta_y - contact_delta.prev_y;
	if (alter > 8 && alter < 20) curr_delta = delta_1;
	else if (alter >= 20 && alter <= 45) curr_delta = delta_2;
	else if (alter > 45) curr_delta = delta_3;
	*/
	if (name(9).point(0).y <= 280) return;
	for (int i = 0; i <= 9; i++) {
		name(i).move(0, curr_delta);
	}
	for (int i = 0; i <= 9; i++) {
		detach(name(i));
		if (name(i).point(0).y >= 280 && name(i).point(0).y <= 376 + 3 * 48) attach(name(i));
	}
	redraw();
}
void Phone::cb_dw(Address, Address pw) {
	reference_to<Phone>(pw).dw();
}
void Phone::dw() {
	PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\lock.wav"), NULL, SND_FILENAME | SND_ASYNC);
	int curr_delta = -48;
	/*
	int alter = -(contact_delta.delta_y - contact_delta.prev_y);
	if (alter > 8 && alter < 20) curr_delta = delta_1;
	else if (alter >= 20 && alter <= 45) curr_delta = delta_2;
	else if (alter > 45) curr_delta = delta_3;
	*/
	if (name(0).point(0).y >= 280 || name(9).point(0).y < 280) return;
	for (int i = 0; i <= 9; i++) {
		name(i).move(0, -curr_delta);
	}
	for (int i = 0; i <= 9; i++) {
		detach(name(i));
		if (name(i).point(0).y >= 280 && name(i).point(0).y <= 376 + 3 * 48) attach(name(i));
	}
	redraw();
}
void Phone::cb_blank(Address, Address pw) {
	reference_to<Phone>(pw).blank();
}
void Phone::blank() {
	return;
}
void Phone::cb_contact(Address, Address pw) {
	reference_to<Phone>(pw).contact();
}
void Phone::contact() {
	PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\lock.wav"), NULL, SND_FILENAME | SND_ASYNC);
	app_open = true;
	Screen_status = inapp;
	attach_contact();
	detach(Desktop);
	detach(call_contact);
	time_refresh_2 = true;
	detach(time_display_2);
	redraw();
	string search = search_name.get_string();
	searching(search);
	/*--------function and buttons waiting*/
}
void Phone::searching(string s) {
	cout << "searched\n";
	if (s == "") return;
	/*
	name_0(Point(114, 280), "Bruno Bucciarati"),
		name_1(Point(114, 328), "Dio Brando"),
		name_2(Point(114, 376), "Guido Mista"),
		name_3(Point(114, 376 + 48), "Higashikata Josuke"),
		name_4(Point(114, 376 + 2 * 48), "King Crimson"),
		name_5(Point(114, 376 + 3 * 48), "Kujo Jotaro"),
		name_6(Point(114, 376 + 4 * 48), "Risotto Nero"),
		name_7(Point(114, 376 + 5 * 48), "Trish Una"),
		name_8(Point(114, 376 + 6 * 48), "Zeppeli Caesar"),
		name_9(Point(114, 376 + 7 * 48), "Zoootopia"),
		*/
	for (int i = 0; i <= 9; i++) {
		name(i).set(114, 280 - 48);
		detach(name(i));
	}

	switch (s[0]) {
	case 'b':
	case 'B': name(0).set(114, 280); attach(name_0); break;
	case 'd': 
	case 'D': name(1).set(114, 280); attach(name_1); break;
	case 'g':
	case 'G': name(2).set(114, 280); attach(name_2); break;
	case 'h':
	case 'H': name(3).set(114, 280); attach(name_3); break;
	case 'k':
	case 'K': name(4).set(114, 280); name(5).set(114, 328); attach(name_4); attach(name_5); break;
	case 'r':
	case 'R': name(6).set(114, 280); attach(name_6); break;
	case 't':
	case 'T': name(7).set(114, 280); attach(name_7); break;
	case 'z':
	case 'Z': name(8).set(114, 280); name(9).set(114, 328); attach(name_8); attach(name_9);  break;
	}
	redraw();
}
void Phone::unlock()
{
	PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\unlock.wav"), NULL, SND_FILENAME | SND_ASYNC);
	detach_unlock(4);
	if (app_open) {
		Screen_status = inapp;
		attach_contact();
		redraw();
	}
	else {
		attach(call_contact);
		attach(Desktop);
		Screen_status = desktop;
		Current_Screen_state.put("Desktop");
		attach(time_display_2);
		for (;;) {
			time_display_2.get_time();
			redraw();
			wait();
			show();
			if (time_refresh_2) { time_refresh_2 = false; break; }
		}
	}
}
void Phone::detach_unlock(int i)
{
	for (int i = 0; i < 10; i++) {
		lock_condition[i] = 1;
	}
	sss.str("");
	password[0] = 0;
	password[1] = 0;
	password[2] = 0;
	password[3] = 0;
	lock_count = 0;
	detach(passdelete);
	detach(pass_title(i));
	detach(pass_0);
	detach(pass_1);
	detach(pass_2);
	detach(pass_3);
	detach(pass_4);
	detach(pass_5);
	detach(pass_6);
	detach(pass_7);
	detach(pass_8);
	detach(pass_9);
	detach(Password);
}
void Phone::attach_unlock()
{
	attach(passdelete);
	attach(pass_0);
	attach(pass_1);
	attach(pass_2);
	attach(pass_3);
	attach(pass_4);
	attach(pass_5);
	attach(pass_6);
	attach(pass_7);
	attach(pass_8);
	attach(pass_9);
	attach(Password);
}
Image& Phone::show_sound(int i) {
	switch (i) {
	case 0: return Sound_screen_0;
	case 1: return Sound_screen_1;
	case 2: return Sound_screen_2;
	case 3: return Sound_screen_3;
	case 4: return Sound_screen_4;
	case 5: return Sound_screen_5;
	case 6: return Sound_screen_6;
	case 7: return Sound_screen_7;
	case 8: return Sound_screen_8;
	case 9: return Sound_screen_9;
	case 10: return Sound_screen_10;
	case 11: return Sound_screen_11;
	case 12: return Sound_screen_12;
	case 13: return Sound_screen_13;
	case 14: return Sound_screen_14;
	case 15: return Sound_screen_15;
	case 16: return Sound_screen_16;
	}
}
void Phone::cb_switchon(Address, Address pw)
{
	reference_to<Phone>(pw).switchon();
}
void Phone::switchon()
{
	if (Screen_status == desktop) {
		time_refresh_2 = true;
	}
	//time_refresh = true;
	if (power_off_try == 1) {
		second_count[0] = clock();
		power_off_check++;
		power_off_try = -power_off_try;
	}
	else {
		second_count[1] = clock();
		power_off_check++;
		power_off_try = -power_off_try;
	}
	if (power_off_check == 2) {
		power_off_check = 0;
		double t = (double)(second_count[1] - second_count[0]) / CLOCKS_PER_SEC;
		cout << t << endl;
		if (t < 1.5) screen_alter();
		else ask_power_off();
	}
}
void Phone::pro() {
	attach(pro__0); redraw(); wait(); show(); Sleep(10);
	attach(pro_0); redraw(); wait(); show(); Sleep(10);
	attach(pro_1); redraw(); wait(); show(); Sleep(10);
	attach(pro_2); redraw(); wait(); show(); Sleep(10);
	attach(pro_3); redraw(); wait(); show(); Sleep(10);
	attach(pro_4); redraw(); wait(); show(); Sleep(10);
	attach(pro_5); redraw(); wait(); show(); Sleep(10);
	attach(pro_6); redraw(); wait(); show(); Sleep(10);
	attach(pro_7); redraw(); wait(); show(); Sleep(10);
	attach(pro_8); redraw(); wait(); show(); Sleep(10);
	attach(pro_9); redraw(); wait(); show(); Sleep(10);
	attach(pro_10); redraw(); wait(); show(); Sleep(10);
	attach(pro_11); redraw(); wait(); show(); Sleep(10);
	attach(pro_12); redraw(); wait(); show(); Sleep(10);
	attach(pro_13); redraw(); wait(); show(); Sleep(10);
	attach(pro_14); redraw(); wait(); show(); Sleep(10);
	attach(pro_15); redraw(); wait(); show(); Sleep(10);
	attach(pro_16); redraw(); wait(); show(); Sleep(10);
	attach(pro_17); redraw(); wait(); show(); Sleep(10);
	attach(pro_18); redraw(); wait(); show(); Sleep(10);
	attach(pro_19); redraw(); wait(); show(); Sleep(10);
	attach(pro_20); redraw(); wait(); show(); Sleep(10);
	attach(pro_21); redraw(); wait(); show(); Sleep(10);
	attach(pro_22); redraw(); wait(); show(); Sleep(10);
	attach(pro_23); redraw(); wait(); show(); Sleep(10);
	attach(pro_24); redraw(); wait(); show(); Sleep(10);
	attach(pro_25); redraw(); wait(); show(); Sleep(10);
	attach(pro_26); redraw(); wait(); show(); Sleep(10);
	attach(pro_27); redraw(); wait(); show(); Sleep(10);
	attach(pro_28); redraw(); wait(); show(); Sleep(10);
	attach(pro_29); redraw(); wait(); show(); Sleep(10);
	attach(pro_30); redraw(); wait(); show(); Sleep(10);
	attach(pro_31); redraw(); wait(); show(); Sleep(10);
	attach(pro_32); redraw(); wait(); show(); Sleep(10);
	attach(pro_33); redraw(); wait(); show(); Sleep(10);
	attach(pro_34); redraw(); wait(); show(); Sleep(10);
	attach(pro_35); redraw(); wait(); show(); Sleep(10);
	attach(pro_36); redraw(); wait(); show(); Sleep(10);
	attach(pro_37); redraw(); wait(); show(); Sleep(10);
	attach(pro_38); redraw(); wait(); show(); Sleep(10);
	attach(pro_39); redraw(); wait(); show(); Sleep(10);
	attach(pro_40); redraw(); wait(); show(); Sleep(10);
	attach(pro_41); redraw(); wait(); show(); Sleep(10);
	attach(pro_42); redraw(); wait(); show(); Sleep(10);
	attach(pro_43); redraw(); wait(); show(); Sleep(10);
	attach(pro_44); redraw(); wait(); show(); Sleep(10);
	attach(pro_45); redraw(); wait(); show(); Sleep(10);
	attach(pro_46); redraw(); wait(); show(); Sleep(10);
	attach(pro_47); redraw(); wait(); show(); Sleep(10);
	attach(pro_48); redraw(); wait(); show(); Sleep(10);
	attach(pro_49); redraw(); wait(); show(); Sleep(10);
	attach(pro_50); redraw(); wait(); show(); Sleep(10);
	attach(pro_51); redraw(); wait(); show(); Sleep(10);
	attach(pro_52); redraw(); wait(); show(); Sleep(10);
	attach(pro_53); redraw(); wait(); show(); Sleep(10);
	attach(pro_54); redraw(); wait(); show(); Sleep(10);
	attach(pro_55); redraw(); wait(); show(); Sleep(10);
	attach(pro_56); redraw(); wait(); show(); Sleep(10);
	attach(pro_57); redraw(); wait(); show(); Sleep(10);
	attach(pro_58); redraw(); wait(); show(); Sleep(10);
	attach(pro_59); redraw(); wait(); show(); Sleep(10);
	attach(pro_60); redraw(); wait(); show(); Sleep(10);
	attach(pro_61); redraw(); wait(); show(); Sleep(10);
	attach(pro_62); redraw(); wait(); show(); Sleep(10);
	attach(pro_63); redraw(); wait(); show(); Sleep(10);
	attach(pro_64); redraw(); wait(); show(); Sleep(10);
	attach(pro_65); redraw(); wait(); show(); Sleep(10);
	attach(pro_66); redraw(); wait(); show(); Sleep(10);
	attach(pro_67); redraw(); wait(); show(); Sleep(10);
	attach(pro_68); redraw(); wait(); show(); Sleep(10);
	attach(pro_69); redraw(); wait(); show(); Sleep(10);
	attach(pro_70); redraw(); wait(); show(); Sleep(10);
	attach(pro_71); redraw(); wait(); show(); Sleep(10);
	attach(pro_72); redraw(); wait(); show(); Sleep(10);
	attach(pro_73); redraw(); wait(); show(); Sleep(10);
	attach(pro_74); redraw(); wait(); show(); Sleep(10);
	attach(pro_75); redraw(); wait(); show(); Sleep(10);
	attach(pro_76); redraw(); wait(); show(); Sleep(10);
	attach(pro_77); redraw(); wait(); show(); Sleep(10);
	attach(pro_78); redraw(); wait(); show(); Sleep(10);
	attach(pro_79); redraw(); wait(); show(); Sleep(10);
	attach(pro_80); redraw(); wait(); show(); Sleep(10);
	attach(pro_81); redraw(); wait(); show(); Sleep(10);
	attach(pro_82); redraw(); wait(); show(); Sleep(10);
	attach(pro_83); redraw(); wait(); show(); Sleep(10);
	attach(pro_84); redraw(); wait(); show(); Sleep(10);
	attach(pro_85); redraw(); wait(); show(); Sleep(10);
	attach(pro_86); redraw(); wait(); show(); Sleep(10);
	attach(pro_87); redraw(); wait(); show(); Sleep(10);
	attach(pro_88); redraw(); wait(); show(); Sleep(10);
	attach(pro_89); redraw(); wait(); show(); Sleep(10);
	attach(pro_90); redraw(); wait(); show(); Sleep(10);
	attach(pro_91); redraw(); wait(); show(); Sleep(10);
	attach(pro_92); redraw(); wait(); show(); Sleep(10);
	attach(pro_93); redraw(); wait(); show(); Sleep(10);
	attach(pro_94); redraw(); wait(); show(); Sleep(10);
	attach(pro_95); redraw(); wait(); show(); Sleep(10);
	attach(pro_96); redraw(); wait(); show(); Sleep(10);
	attach(pro_97); redraw(); wait(); show(); Sleep(10);
	attach(pro_98); redraw(); wait(); show(); Sleep(10);
	attach(pro_99); redraw(); wait(); show(); Sleep(10);
	attach(pro_100); redraw(); wait(); show(); Sleep(10);
	attach(pro_101); redraw(); wait(); show(); Sleep(10);
	attach(pro_102); redraw(); wait(); show(); Sleep(10);
	attach(pro_103); redraw(); wait(); show(); Sleep(10);
	attach(pro_104); redraw(); wait(); show(); Sleep(10);
	attach(pro_105); redraw(); wait(); show(); Sleep(10);
	attach(pro_106); redraw(); wait(); show(); Sleep(10);
	attach(pro_107); redraw(); wait(); show(); Sleep(10);
	attach(pro_108); redraw(); wait(); show(); Sleep(10);
	attach(pro_109); redraw(); wait(); show(); Sleep(10);
	attach(pro_110); redraw(); wait(); show(); Sleep(10);
	attach(pro_111); redraw(); wait(); show(); Sleep(10);

}
void Phone::depro() {
	detach(pro__0);
	detach(pro_0);
	detach(pro_1);
	detach(pro_2);
	detach(pro_3);
	detach(pro_4);
	detach(pro_5);
	detach(pro_6);
	detach(pro_7);
	detach(pro_8);
	detach(pro_9);
	detach(pro_10);
	detach(pro_11);
	detach(pro_12);
	detach(pro_13);
	detach(pro_14);
	detach(pro_15);
	detach(pro_16);
	detach(pro_17);
	detach(pro_18);
	detach(pro_19);
	detach(pro_20);
	detach(pro_21);
	detach(pro_22);
	detach(pro_23);
	detach(pro_24);
	detach(pro_25);
	detach(pro_26);
	detach(pro_27);
	detach(pro_28);
	detach(pro_29);
	detach(pro_30);
	detach(pro_31);
	detach(pro_32);
	detach(pro_33);
	detach(pro_34);
	detach(pro_35);
	detach(pro_36);
	detach(pro_37);
	detach(pro_38);
	detach(pro_39);
	detach(pro_40);
	detach(pro_41);
	detach(pro_42);
	detach(pro_43);
	detach(pro_44);
	detach(pro_45);
	detach(pro_46);
	detach(pro_47);
	detach(pro_48);
	detach(pro_49);
	detach(pro_50);
	detach(pro_51);
	detach(pro_52);
	detach(pro_53);
	detach(pro_54);
	detach(pro_55);
	detach(pro_56);
	detach(pro_57);
	detach(pro_58);
	detach(pro_59);
	detach(pro_60);
	detach(pro_61);
	detach(pro_62);
	detach(pro_63);
	detach(pro_64);
	detach(pro_65);
	detach(pro_66);
	detach(pro_67);
	detach(pro_68);
	detach(pro_69);
	detach(pro_70);
	detach(pro_71);
	detach(pro_72);
	detach(pro_73);
	detach(pro_74);
	detach(pro_75);
	detach(pro_76);
	detach(pro_77);
	detach(pro_78);
	detach(pro_79);
	detach(pro_80);
	detach(pro_81);
	detach(pro_82);
	detach(pro_83);
	detach(pro_84);
	detach(pro_85);
	detach(pro_86);
	detach(pro_87);
	detach(pro_88);
	detach(pro_89);
	detach(pro_90);
	detach(pro_91);
	detach(pro_92);
	detach(pro_93);
	detach(pro_94);
	detach(pro_95);
	detach(pro_96);
	detach(pro_97);
	detach(pro_98);
	detach(pro_99);
	detach(pro_100);
	detach(pro_101);
	detach(pro_102);
	detach(pro_103);
	detach(pro_104);
	detach(pro_105);
	detach(pro_106);
	detach(pro_107);
	detach(pro_108);
	detach(pro_109);
	detach(pro_110);
	detach(pro_111);

}
void Phone::ask_power_off() {
	PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\beautifulcock.wav"), NULL, SND_FILENAME | SND_ASYNC);
	switch (Screen_status) {
	case poweroff:
		Screen_status = starting; detach(Closed_screen); attach(Starting_screen); Current_Screen_state.put("starting......"); redraw(); wait(); show(); time_refresh = false;
		Sleep(200);
		detach(Starting_screen); attach(Starting_p0); redraw(); wait(); show();
		Sleep(450);
		pro();
		attach(Starting_p3); redraw(); wait(); show();
		Sleep(100);
		/*
		detach(Starting_p0); attach(Starting_p1); redraw(); wait(); show();
		Sleep(450);
		detach(Starting_p1); attach(Starting_p2); redraw(); wait(); show();
		Sleep(450);
		detach(Starting_p2); attach(Starting_p3); redraw(); wait(); show();
		Sleep(450);
		*/
		Screen_status = locked; detach(Starting_p3); depro(); attach(Lock_screen); attach(time_display);
		Current_Screen_state.put("locked open"); redraw();
		redraw(); wait(); show();
		for (;;) {
			time_display.get_time();
			redraw();
			wait();
			show();
			if (time_refresh) { time_refresh = false; break; }
		}
		break;
	case locked:
		Screen_status = poweroff; detach(Lock_screen); detach(time_display); attach(Closed_screen); Current_Screen_state.put("poweroff"); redraw(); break;
	case closed:
		Screen_status = poweroff; Current_Screen_state.put("poweroff"); redraw(); break;
	case passwording:
		Screen_status = poweroff; detach_unlock(lock_count); attach(Closed_screen); Current_Screen_state.put("poweroff"); redraw(); break;
	case desktop:
		Screen_status = poweroff; detach(call_contact); detach(Desktop); detach(time_display_2); attach(Closed_screen); Current_Screen_state.put("poweroff"); redraw(); break;
	case inapp:
		Screen_status = poweroff; detach_contact();
		app_open = false; attach(Closed_screen); Current_Screen_state.put("poweroff"); redraw(); break;
	}
	contact_ini();
}
void Phone::screen_alter()
{
	PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\lock.wav"), NULL, SND_FILENAME | SND_ASYNC);

	switch (Screen_status) {
		/*
	case poweroff:
		Screen_status = starting; detach(Closed_screen); attach(Starting_screen); Current_Screen_state.put("starting......"); redraw(); wait(); show();
		Sleep(2000); Screen_status = locked; detach(Starting_screen); attach(Lock_screen);
		Current_Screen_state.put("locked open"); redraw();
		break;
		*/
	case locked:
		Screen_status = closed; detach(Lock_screen); detach(time_display); attach(Closed_screen); Current_Screen_state.put("closed"); redraw(); break;
	case closed:
		Screen_status = locked; detach(Closed_screen); attach(Lock_screen); attach(time_display); time_refresh = false;
		Current_Screen_state.put("locked open"); redraw();
		for (;;) {
			time_display.get_time();
			redraw();
			wait();
			show();
			if (time_refresh) { time_refresh = false; break; }
		}
		break;
	case passwording:
		Screen_status = closed; detach_unlock(lock_count); attach(Closed_screen); Current_Screen_state.put("closed"); redraw(); break;
	case desktop:
		Screen_status = closed; detach(call_contact); detach(Desktop); detach(time_display_2); attach(Closed_screen); Current_Screen_state.put("closed"); redraw(); break;
	case inapp:
		Screen_status = closed;	detach_contact();
		attach(Closed_screen); Current_Screen_state.put("closed"); redraw(); break;
	}
}
void Phone::cb_home(Address, Address pw)
{
	reference_to<Phone>(pw).home();
}
void Phone::home()
{
	time_refresh = true;
	PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\lock.wav"), NULL, SND_FILENAME | SND_ASYNC);
	switch (Screen_status) {
	case closed: Screen_status = passwording; detach(Closed_screen); attach_unlock(); 
		Current_Screen_state.put("passwording..."); redraw(); break;
	case locked: Screen_status = passwording; detach(Lock_screen); detach(time_display); attach_unlock();
		Current_Screen_state.put("passwording..."); redraw(); break;
	case inapp: Screen_status = desktop; detach(pg_up); detach(pg_dw); detach(draging);
		for (int i = 0; i <= 9; i++) {
			detach(name(i));
		}
		detach(Contact); attach(Desktop); attach(call_contact); attach(time_display_2); app_open = false; time_refresh_2 = false;
		Current_Screen_state.put("Desktop"); redraw(); 
		for (;;) {
			time_display_2.get_time();
			redraw();
			wait();
			show();
			if (time_refresh_2) { time_refresh_2 = false; break; }
		}
		break;
	}
}
void Phone::cb_mute(Address, Address pw)
{
	reference_to<Phone>(pw).mute();
}
void Phone::mute()
{
	if (Screen_status != poweroff || Screen_status != closed) {
		PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\lock.wav"), NULL, SND_FILENAME | SND_ASYNC);
		switch (Sound_status) {
		case muted: Sound_status = Sound_saved; attach(show_sound(Sound_status)); redraw(); wait(); show();
			Sleep(400); detach(show_sound(Sound_status)); break;
		default: Sound_status = muted; attach(show_sound(muted)); redraw(); wait(); show();
			Sleep(400); detach(show_sound(muted)); break;
		}
	};
	return;
}
void Phone::cb_soundup(Address, Address pw)
{
	reference_to<Phone>(pw).soundup();
}
void Phone::soundup()
{
	stringstream in;
	if (Screen_status != poweroff && Screen_status != closed) {
		PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\lock.wav"), NULL, SND_FILENAME | SND_ASYNC);
		switch (Sound_status) {
		case muted: Sound_status = Sound_saved; attach(show_sound(Sound_status)); Current_Sound_state.put("muted"); redraw(); wait(); show();
			Sleep(400); detach(show_sound(Sound_status)); break;
		case 16: attach(show_sound(16)); in << Sound_status; Current_Sound_state.put(in.str()); redraw(); wait(); show();
			Sleep(400); detach(show_sound(16)); break;
		default: Sound_saved = (Sound_status += 1); attach(show_sound(Sound_status)); in << Sound_status; Current_Sound_state.put(in.str()); redraw(); wait(); show();
			Sleep(400); detach(show_sound(Sound_status)); break;
		}
	}
}//wait:静音后点击音量变化 为saved++或-- 未完成
void Phone::cb_sounddn(Address, Address pw)
{
	reference_to<Phone>(pw).sounddn();
}
void Phone::sounddn()
{
	stringstream in;
	if (Screen_status != poweroff && Screen_status != closed) {
		PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\lock.wav"), NULL, SND_FILENAME | SND_ASYNC);
		switch (Sound_status) {
		case muted: Sound_status = Sound_saved; attach(show_sound(Sound_status)); in << Sound_status; Current_Sound_state.put(in.str()); redraw(); wait(); show();
			Sleep(400); detach(show_sound(Sound_status)); break;
		case 1: Sound_status = muted; attach(show_sound(muted)); Current_Sound_state.put("muted"); redraw(); wait(); show();
			Sleep(400); detach(show_sound(muted)); break;
		default: Sound_saved = (Sound_status -= 1); attach(show_sound(Sound_status)); in << Sound_status; Current_Sound_state.put(in.str()); redraw(); wait(); show();
			Sleep(400); detach(show_sound(Sound_status)); break;
		}
	}
}
/*
void Phone::cb_pass0(Address, Address pw)
{
	reference_to<Phone>(pw).pass0();
}
void Phone::pass0()
{
	if (lock_condition[0] > 0) {
		lock_count++;
		attach(p0);
		attach(pass_title(lock_count));
		detach(pass_title(lock_count - 1));
		password[lock_count - 1] = 0;
		sss << 0;
		password_state.put(sss.str());
	}
	else detach(p0);
	lock_condition[0] = -lock_condition[0];
	redraw();
	wait();
	show();

	if (lock_count == 4 && lock_condition[0] > 0) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			detach_passbutton();
			redraw();
			Sleep(1000);
			redraw();
			attach_passbutton();
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass1(Address, Address pw)
{
	reference_to<Phone>(pw).pass1();
}
void Phone::pass1()
{
	if (lock_condition[1] > 0) {
		lock_count++;
		attach(p1);
		attach(pass_title(lock_count));
		detach(pass_title(lock_count - 1));
		password[lock_count - 1] = 1;
		sss << 1;
		password_state.put(sss.str());
	}
	else detach(p1);
	lock_condition[1] = -lock_condition[1];
	redraw();
	wait();
	show();

	if (lock_count == 4 && lock_condition[1] > 0) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			detach_passbutton();
			Sleep(1000);
			attach_passbutton();
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass2(Address, Address pw)
{
	reference_to<Phone>(pw).pass2();
}
void Phone::pass2()
{
	if (lock_condition[2] > 0) {
		lock_count++;
		attach(p2);
		attach(pass_title(lock_count));
		detach(pass_title(lock_count - 1));
		password[lock_count - 1] = 2;
		sss << 2;
		password_state.put(sss.str());
	}
	else detach(p2);
	lock_condition[2] = -lock_condition[2];
	redraw();
	wait();
	show();

	if (lock_count == 4 && lock_condition[2] > 0) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			detach_passbutton();
			Sleep(1000);
			attach_passbutton();
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass3(Address, Address pw)
{
	reference_to<Phone>(pw).pass3();
}
void Phone::pass3()
{
	if (lock_condition[3] > 0) {
		lock_count++;
		attach(p3);
		attach(pass_title(lock_count));
		detach(pass_title(lock_count - 1));
		password[lock_count - 1] = 3;
		sss << 3;
		password_state.put(sss.str());
	}
	else detach(p3);
	lock_condition[3] = -lock_condition[3];
	redraw();
	wait();
	show();

	if (lock_count == 4 && lock_condition[3] > 0) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			detach_passbutton();
			Sleep(1000);
			attach_passbutton();
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass4(Address, Address pw)
{
	reference_to<Phone>(pw).pass4();
}
void Phone::pass4()
{
	if (lock_condition[4] > 0) {
		lock_count++;
		attach(p4);
		attach(pass_title(lock_count));
		detach(pass_title(lock_count - 1));
		password[lock_count - 1] = 4;
		sss << 4;
		password_state.put(sss.str());
	}
	else detach(p4);
	lock_condition[4] = -lock_condition[4];
	redraw();
	wait();
	show();

	if (lock_count == 4 && lock_condition[4] > 0) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			detach_passbutton();
			Sleep(1000);
			attach_passbutton();
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass5(Address, Address pw)
{
	reference_to<Phone>(pw).pass5();
}
void Phone::pass5()
{
	if (lock_condition[5] > 0) {
		lock_count++;
		attach(p5);
		attach(pass_title(lock_count));
		detach(pass_title(lock_count - 1));
		password[lock_count - 1] = 5;
		sss << 5;
		password_state.put(sss.str());
	}
	else detach(p5);
	lock_condition[5] = -lock_condition[5];
	redraw();
	wait();
	show();

	if (lock_count == 4 && lock_condition[5] > 0) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			detach_passbutton();
			Sleep(1000);
			attach_passbutton();
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass6(Address, Address pw)
{
	reference_to<Phone>(pw).pass6();
}
void Phone::pass6()
{
	if (lock_condition[6] > 0) {
		lock_count++;
		attach(p6);
		attach(pass_title(lock_count));
		detach(pass_title(lock_count - 1));
		password[lock_count - 1] = 6;
		sss << 6;
		password_state.put(sss.str());
	}
	else detach(p6);
	lock_condition[6] = -lock_condition[6];
	redraw();
	wait();
	show();

	if (lock_count == 4 && lock_condition[6] > 0) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			detach_passbutton();
			Sleep(1000);
			attach_passbutton();
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass7(Address, Address pw)
{
	reference_to<Phone>(pw).pass7();
}
void Phone::pass7()
{
	if (lock_condition[7] > 0) {
		lock_count++;
		attach(p7);
		attach(pass_title(lock_count));
		detach(pass_title(lock_count - 1));
		password[lock_count - 1] = 7;
		sss << 7;
		password_state.put(sss.str());
	}
	else detach(p7);
	lock_condition[7] = -lock_condition[7];
	redraw();
	wait();
	show();

	if (lock_count == 4 && lock_condition[7] > 0) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			detach_passbutton();
			Sleep(1000);
			attach_passbutton();
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass8(Address, Address pw)
{
	reference_to<Phone>(pw).pass8();
}
void Phone::pass8()
{
	if (lock_condition[8] > 0) {
		lock_count++;
		attach(p8);
		attach(pass_title(lock_count));
		detach(pass_title(lock_count - 1));
		password[lock_count - 1] = 8;
		sss << 8;
		password_state.put(sss.str());
	}
	else detach(p8);
	lock_condition[8] = -lock_condition[8];
	redraw();
	wait();
	show();

	if (lock_count == 4 && lock_condition[8] > 0) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			detach_passbutton();
			Sleep(1000);
			attach_passbutton();
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass9(Address, Address pw)
{
	reference_to<Phone>(pw).pass9();
}
void Phone::pass9()
{
	if (lock_condition[9] > 0) {
		lock_count++;
		attach(p9);
		attach(pass_title(lock_count));
		detach(pass_title(lock_count - 1));
		password[lock_count - 1] = 9;
		sss << 9;
		password_state.put(sss.str());
	}
	else detach(p9);
	lock_condition[9] = -lock_condition[9];
	redraw();
	wait();
	show();

	if (lock_count == 4 && lock_condition[9] > 0) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			detach_passbutton();
			Sleep(1000);
			attach_passbutton();
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
*/
void Phone::cb_pass_delete(Address, Address pw)
{
	reference_to<Phone>(pw).pass_delete();
}
void Phone::pass_delete()
{
	PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\lock.wav"), NULL, SND_FILENAME | SND_ASYNC);

	if (lock_count) {
		lock_count--;
		attach(pass_title(lock_count));
		detach(pass_title(lock_count + 1));
		sss.str("");
		for (int i = 0; i < lock_count; i++) {
			sss << password[i];
		}
		password_state.put(sss.str());
		redraw();
	}
	else {
		detach_unlock(0);
		Screen_status = locked;
		attach(Lock_screen);
		attach(time_display);
		Current_Screen_state.put("locked open");
		for (;;) {
			time_display.get_time();
			redraw();
			wait();
			show();
			if (time_refresh) { time_refresh = false; break; }
		}
		redraw();
	}
}

void Phone::cb_pass0(Address, Address pw)
{
	reference_to<Phone>(pw).pass0();
}
void Phone::pass0()
{
	lock_count++;
	attach(pass_title(lock_count));
	detach(pass_title(lock_count - 1));
	password[lock_count - 1] = 0;
	sss << 0;
	password_state.put(sss.str());
	redraw();
	wait();
	show();

	if (lock_count == 4) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\pass_wrong.wav"), NULL, SND_FILENAME | SND_ASYNC);
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			Sleep(1000);
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass1(Address, Address pw)
{
	reference_to<Phone>(pw).pass1();
}
void Phone::pass1()
{
	lock_count++;
	attach(pass_title(lock_count));
	detach(pass_title(lock_count - 1));
	password[lock_count - 1] = 1;
	sss << 1;
	password_state.put(sss.str());
	redraw();
	wait();
	show();

	if (lock_count == 4) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\pass_wrong.wav"), NULL, SND_FILENAME | SND_ASYNC);
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			Sleep(1000);
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass2(Address, Address pw)
{
	reference_to<Phone>(pw).pass2();
}
void Phone::pass2()
{
	lock_count++;
	attach(pass_title(lock_count));
	detach(pass_title(lock_count - 1));
	password[lock_count - 1] = 2;
	sss << 2;
	password_state.put(sss.str());
	redraw();
	wait();
	show();

	if (lock_count == 4) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\pass_wrong.wav"), NULL, SND_FILENAME | SND_ASYNC);
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			Sleep(1000);
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass3(Address, Address pw)
{
	reference_to<Phone>(pw).pass3();
}
void Phone::pass3()
{
	lock_count++;
	attach(pass_title(lock_count));
	detach(pass_title(lock_count - 1));
	password[lock_count - 1] = 3;
	sss << 3;
	password_state.put(sss.str());
	redraw();
	wait();
	show();

	if (lock_count == 4) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\pass_wrong.wav"), NULL, SND_FILENAME | SND_ASYNC);
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			Sleep(1000);
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass4(Address, Address pw)
{
	reference_to<Phone>(pw).pass4();
}
void Phone::pass4()
{
	lock_count++;
	attach(pass_title(lock_count));
	detach(pass_title(lock_count - 1));
	password[lock_count - 1] = 4;
	sss << 4;
	password_state.put(sss.str());
	redraw();
	wait();
	show();

	if (lock_count == 4) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\pass_wrong.wav"), NULL, SND_FILENAME | SND_ASYNC);
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			Sleep(1000);
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass5(Address, Address pw)
{
	reference_to<Phone>(pw).pass5();
}
void Phone::pass5()
{
	lock_count++;
	attach(pass_title(lock_count));
	detach(pass_title(lock_count - 1));
	password[lock_count - 1] = 5;
	sss << 5;
	password_state.put(sss.str());
	redraw();
	wait();
	show();

	if (lock_count == 4) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\pass_wrong.wav"), NULL, SND_FILENAME | SND_ASYNC);
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			Sleep(1000);
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass6(Address, Address pw)
{
	reference_to<Phone>(pw).pass6();
}
void Phone::pass6()
{
	lock_count++;
	attach(pass_title(lock_count));
	detach(pass_title(lock_count - 1));
	password[lock_count - 1] = 6;
	sss << 6;
	password_state.put(sss.str());
	redraw();
	wait();
	show();

	if (lock_count == 4) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\pass_wrong.wav"), NULL, SND_FILENAME | SND_ASYNC);
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			Sleep(1000);
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass7(Address, Address pw)
{
	reference_to<Phone>(pw).pass7();
}
void Phone::pass7()
{
	lock_count++;
	attach(pass_title(lock_count));
	detach(pass_title(lock_count - 1));
	password[lock_count - 1] = 7;
	sss << 7;
	password_state.put(sss.str());
	redraw();
	wait();
	show();

	if (lock_count == 4) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\pass_wrong.wav"), NULL, SND_FILENAME | SND_ASYNC);
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			Sleep(1000);
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass8(Address, Address pw)
{
	reference_to<Phone>(pw).pass8();
}
void Phone::pass8()
{
	lock_count++;
	attach(pass_title(lock_count));
	detach(pass_title(lock_count - 1));
	password[lock_count - 1] = 8;
	sss << 8;
	password_state.put(sss.str());
	redraw();
	wait();
	show();

	if (lock_count == 4) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\pass_wrong.wav"), NULL, SND_FILENAME | SND_ASYNC);
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			Sleep(1000);
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass9(Address, Address pw)
{
	reference_to<Phone>(pw).pass9();
}
void Phone::pass9()
{
	lock_count++;
	attach(pass_title(lock_count));
	detach(pass_title(lock_count - 1));
	password[lock_count - 1] = 9;
	sss << 9;
	password_state.put(sss.str());
	redraw();
	wait();
	show();

	if (lock_count == 4) {
		password_state.put(sss.str());
		if (password[0] == pass_word[0] && password[1] == pass_word[1] && password[2] == pass_word[2] && password[3] == pass_word[3]) {
			unlock();
		}
		else {
			PlaySound(TEXT("D:\\C++\\Phone_Project\\images\\pass_wrong.wav"), NULL, SND_FILENAME | SND_ASYNC);
			lock_count = 0;
			attach(pass_titlewrong);
			password_state.put("Wrong!!!");
			redraw();
			wait();
			show();
			Sleep(1000);
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
}; // of namespace Graph_lib
