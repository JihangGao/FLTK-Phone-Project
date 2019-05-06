
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//
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
//------------------------------------------------------------------------------
Phone::Phone() :
	Window(Point(100, 100), 700, 700, "Phone"),
	Phone_fraction(Point(0, 0), "D:\\C++\\Phone_Project\\images\\fraction_bg.jpg"),
	//home_button(Point(209, 520), 10, 10, "", cb_back_to_home)
	Lock_button(Point(321, 120), 6, 45, "", cb_switchon),
	Lock_screen(Point(27, 75), "D:\\C++\\Phone_Project\\images\\lock.jpg"),
	Closed_screen(Point(27, 75), "D:\\C++\\Phone_Project\\images\\closed_bg.jpg"),
	Starting_screen(Point(27, 75), "D:\\C++\\Phone_Project\\images\\starting.jpg"),
	Up_button(Point(4, 121), 10, 41, "", cb_soundup),
	Down_button(Point(4, 174), 10, 41, "", cb_sounddn),
	Mute_button(Point(5, 74), 8, 32, "", cb_mute),
	Current_Screen_state(Point(500, 500), 100, 40, "Current Screen State:"),
	Current_Sound_state(Point(500, 400), 100, 40, "Current Sound State:"),
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
	pass_title1(Point(27,135), "D:\\C++\\Phone_Project\\images\\pass_title1.jpg"),
	pass_title2(Point(27, 135), "D:\\C++\\Phone_Project\\images\\pass_title2.jpg"),
	pass_title3(Point(27, 135), "D:\\C++\\Phone_Project\\images\\pass_title3.jpg"),
	pass_title4(Point(27, 135), "D:\\C++\\Phone_Project\\images\\pass_title4.jpg"),
	pass_title0(Point(27, 135), "D:\\C++\\Phone_Project\\images\\pass_title0.jpg"),
	pass_titlewrong(Point(27, 135), "D:\\C++\\Phone_Project\\images\\pass_titlewrong.jpg"),
	Desktop(Point(27, 75), "D:\\C++\\Phone_Project\\images\\desktop.jpg"),
	home_button(Point(139, 578), 50, 50, "home", cb_home),
	password_state(Point(400, 0), 100, 40, "password"),
	passdelete(Point(220, 527), 42, 18, "", cb_pass_delete)
{
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
}
void Phone::unlock()
{
	detach_unlock(4);
	attach(Desktop);
	Screen_status = desktop;
	Current_Screen_state.put("Desktop");
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
	switch (Screen_status) {
		
	case poweroff: Screen_status = starting; detach(Closed_screen); attach(Starting_screen); Current_Screen_state.put("starting......"); redraw(); wait(); show();
				   Sleep(2000); Screen_status = locked; detach(Starting_screen); attach(Lock_screen);  Current_Screen_state.put("locked open"); redraw();
				   break;
	case locked: Screen_status = closed; detach(Lock_screen); attach(Closed_screen); Current_Screen_state.put("closed"); redraw(); break;
	case closed: Screen_status = locked; detach(Closed_screen); attach(Lock_screen); Current_Screen_state.put("locked open"); redraw(); break;
	case passwording: Screen_status = closed; detach_unlock(lock_count); attach(Closed_screen); Current_Screen_state.put("closed"); redraw(); break;
	case desktop: Screen_status = closed; detach(Desktop); attach(Closed_screen); Current_Screen_state.put("closed"); redraw(); break;
	}
}
void Phone::cb_home(Address, Address pw)
{
	reference_to<Phone>(pw).home();
}
void Phone::home()
{
	switch (Screen_status) {
	case closed: Screen_status = passwording; detach(Closed_screen); attach_unlock(); Current_Screen_state.put("passwording..."); redraw(); break;
	case locked: Screen_status = passwording; detach(Lock_screen); attach_unlock(); Current_Screen_state.put("passwording..."); redraw(); break;
	}
}
void Phone::cb_mute(Address, Address pw)
{
	reference_to<Phone>(pw).mute();
}
void Phone::mute()
{
	if (Screen_status != poweroff || Screen_status != closed) {
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
			Sleep(1000);
			detach(pass_titlewrong);
			attach(pass_title0);
			detach(pass_title4);
			sss.str("");
			redraw();
		}
	}
}
void Phone::cb_pass_delete(Address, Address pw)
{
	reference_to<Phone>(pw).pass_delete();
}
void Phone::pass_delete()
{
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
		Current_Screen_state.put("locked open"); 
		redraw();
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
