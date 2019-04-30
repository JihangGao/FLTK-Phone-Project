
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//
#include "GUI.h"
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
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
#define muted 0
namespace Graph_lib {

//------------------------------------------------------------------------------

void Button::attach(Window& win)
{
    pw = new Fl_Button(loc.x, loc.y, width, height, label.c_str());
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
Phone::Phone() :
	Window(Point(100, 100), 700, 700, "Phone"),
	Phone_fraction(Point(0, 0), "D:\\C++\\Phone_Project\\images\\fraction_bg.jpg"),
	//home_button(Point(209, 520), 10, 10, "", cb_back_to_home)
	Lock_button(Point(321, 120), 6, 45, "", cb_switchon),
	Lock_screen(Point(27, 75), "D:\\C++\\Phone_Project\\images\\lock.jpg"),
	Closed_screen(Point(27, 75), "D:\\C++\\Phone_Project\\images\\closed_bg.jpg"),
	Starting_screen(Point(27,75), "D:\\C++\\Phone_Project\\images\\starting.gif"),
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
	Sound_screen_16(Point(117, 271), "D:\\C++\\Phone_Project\\images\\s16.jpg")
{
	attach(Lock_button);
	attach(Up_button);
	attach(Down_button);
	attach(Mute_button);
	attach(Phone_fraction);
	attach(Closed_screen);
	attach(Current_Screen_state);
	attach(Current_Sound_state);
	Current_Screen_state.put("Power off");
	//attach(open_screen);
	Sound_saved = 8;
	Screen_status = poweroff;
	Sound_status = 8;
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
				   Sleep(20000); Screen_status = locked; detach(Starting_screen); attach(Lock_screen);  Current_Screen_state.put("locked open"); redraw();
				   break;
	case locked: Screen_status = closed; detach(Lock_screen); attach(Closed_screen); Current_Screen_state.put("closed"); redraw(); break;
	case closed: Screen_status = locked; detach(Closed_screen); attach(Lock_screen); Current_Screen_state.put("locked open"); redraw(); break;
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
}
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
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
}; // of namespace Graph_lib
