
//
// This is a GUI support code to the chapters 12-16 of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//
#pragma warning(disable:4996)
#include <FL/Fl_GIF_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>
#include "Graph.h"
#include <Windows.h>
#include <time.h> 

//-----------------------------------------------------
int calpow(int i) {
	int sum = 0;
	for (int p = 0; p < i; p++) {
		sum += pow(2, p);
	}
	return sum;
}

namespace Graph_lib {

//------------------------------------------------------------------------------

Shape::Shape() : 
    lcolor(fl_color()),      // default color for lines and characters
    ls(0),                   // default style
    fcolor(Color::invisible) // no fill
{}

//------------------------------------------------------------------------------

void Shape::add(Point p)     // protected
{
    points.push_back(p);
}

//------------------------------------------------------------------------------

void Shape::set_point(int i,Point p)        // not used; not necessary so far
{
    points[i] = p;
}

//------------------------------------------------------------------------------

void Shape::draw_lines() const
{
    if (color().visibility() && 1<points.size())    // draw sole pixel?
        for (unsigned int i=1; i<points.size(); ++i)
            fl_line(points[i-1].x,points[i-1].y,points[i].x,points[i].y);
}

//------------------------------------------------------------------------------

void Shape::draw() const
{
    Fl_Color oldc = fl_color();
    // there is no good portable way of retrieving the current style
    fl_color(lcolor.as_int());            // set color
    fl_line_style(ls.style(),ls.width()); // set style
    draw_lines();
    fl_color(oldc);      // reset color (to previous)
    fl_line_style(0);    // reset line style to default
}

//------------------------------------------------------------------------------


void Shape::move(int dx, int dy)    // move the shape +=dx and +=dy
{
    for (int i = 0; i<points.size(); ++i) {
        points[i].x+=dx;
        points[i].y+=dy;
    }
}

//------------------------------------------------------------------------------

Line::Line(Point p1, Point p2)    // construct a line from two points
{
    add(p1);    // add p1 to this shape
    add(p2);    // add p2 to this shape
}

//------------------------------------------------------------------------------

void Lines::add(Point p1, Point p2)
{
    Shape::add(p1);
    Shape::add(p2);
}

//------------------------------------------------------------------------------

// draw lines connecting pairs of points
void Lines::draw_lines() const
{
    if (color().visibility())
        for (int i=1; i<number_of_points(); i+=2)
            fl_line(point(i-1).x,point(i-1).y,point(i).x,point(i).y);
}

//------------------------------------------------------------------------------

// does two lines (p1,p2) and (p3,p4) intersect?
// if se return the distance of the intersect point as distances from p1
inline pair<double,double> line_intersect(Point p1, Point p2, Point p3, Point p4, bool& parallel) 
{
    double x1 = p1.x;
    double x2 = p2.x;
    double x3 = p3.x;
    double x4 = p4.x;
    double y1 = p1.y;
    double y2 = p2.y;
    double y3 = p3.y;
    double y4 = p4.y;

    double denom = ((y4 - y3)*(x2-x1) - (x4-x3)*(y2-y1));
    if (denom == 0){
        parallel= true;
        return pair<double,double>(0,0);
    }
    parallel = false;
    return pair<double,double>( ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3))/denom,
                                ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3))/denom);
}

//------------------------------------------------------------------------------

//intersection between two line segments
//Returns true if the two segments intersect,
//in which case intersection is set to the point of intersection
bool line_segment_intersect(Point p1, Point p2, Point p3, Point p4, Point& intersection){
   bool parallel;
   pair<double,double> u = line_intersect(p1,p2,p3,p4,parallel);
   if (parallel || u.first < 0 || u.first > 1 || u.second < 0 || u.second > 1) return false;
   intersection.x = p1.x + u.first*(p2.x - p1.x);
   intersection.y = p1.y + u.first*(p2.y - p1.y);
   return true;
}

//------------------------------------------------------------------------------

void Polygon::add(Point p)
{
    int np = number_of_points();

    if (1<np) {    // check that thenew line isn't parallel to the previous one
        if (p==point(np-1)) error("polygon point equal to previous point");
        bool parallel;
        line_intersect(point(np-1),p,point(np-2),point(np-1),parallel);
        if (parallel)
            error("two polygon points lie in a straight line");
    }

    for (int i = 1; i<np-1; ++i) {    // check that new segment doesn't interset and old point
        Point ignore(0,0);
        if (line_segment_intersect(point(np-1),p,point(i-1),point(i),ignore))
            error("intersect in polygon");
    }
    

    Closed_polyline::add(p);
}

//------------------------------------------------------------------------------

void Polygon::draw_lines() const
{
    if (number_of_points() < 3) error("less than 3 points in a Polygon");
    Closed_polyline::draw_lines();
}

//------------------------------------------------------------------------------

void Open_polyline::draw_lines() const
{
    if (fill_color().visibility()) {
        fl_color(fill_color().as_int());
        fl_begin_complex_polygon();
        for(int i=0; i<number_of_points(); ++i){
            fl_vertex(point(i).x, point(i).y);
        }
        fl_end_complex_polygon();
        fl_color(color().as_int());    // reset color
    }
    
    if (color().visibility())
        Shape::draw_lines();
}

//------------------------------------------------------------------------------

void Closed_polyline::draw_lines() const
{
    Open_polyline::draw_lines();    // first draw the "open poly line part"
    // then draw closing line:
    if (color().visibility())
        fl_line(point(number_of_points()-1).x, 
        point(number_of_points()-1).y,
        point(0).x,
        point(0).y);
}

//------------------------------------------------------------------------------

void draw_mark(Point xy, char c)
{
    static const int dx = 4;
    static const int dy = 4;

    string m(1,c);
    fl_draw(m.c_str(),xy.x-dx,xy.y+dy);
}

//------------------------------------------------------------------------------

void Marked_polyline::draw_lines() const
{
    Open_polyline::draw_lines();
    for (int i=0; i<number_of_points(); ++i) 
        draw_mark(point(i),mark[i%mark.size()]);
}

//------------------------------------------------------------------------------

void Rectangle::draw_lines() const
{
    if (fill_color().visibility()) {    // fill
        fl_color(fill_color().as_int());
        fl_rectf(point(0).x,point(0).y,w,h);
    }

    if (color().visibility()) {    // lines on top of fill
        fl_color(color().as_int());
        fl_rect(point(0).x,point(0).y,w,h);
    }
}

//------------------------------------------------------------------------------

Circle::Circle(Point p, int rr)    // center and radius
:r(rr)
{
    add(Point(p.x-r,p.y-r));       // store top-left corner
}

//------------------------------------------------------------------------------

Point Circle::center() const
{
    return Point(point(0).x+r, point(0).y+r);
}

//------------------------------------------------------------------------------

void Circle::draw_lines() const
{
    if (color().visibility())
        fl_arc(point(0).x,point(0).y,r+r,r+r,0,360);
}

//------------------------------------------------------------------------------

void Ellipse::draw_lines() const
{
    if (color().visibility())
        fl_arc(point(0).x,point(0).y,w+w,h+h,0,360);
}

//------------------------------------------------------------------------------

void Text::draw_lines() const
{
    int ofnt = fl_font();
    int osz = fl_size();
    fl_font(fnt.as_int(),fnt_sz);
    fl_draw(lab.c_str(),point(0).x,point(0).y);
    fl_font(ofnt,osz);
}

//------------------------------------------------------------------------------

Axis::Axis(Orientation d, Point xy, int length, int n, string lab) :
    label(Point(0,0),lab)
{
    if (length<0) error("bad axis length");
    switch (d){
    case Axis::x:
    {
        Shape::add(xy); // axis line
        Shape::add(Point(xy.x+length,xy.y));

        if (1<n) {      // add notches
            int dist = length/n;
            int x = xy.x+dist;
            for (int i = 0; i<n; ++i) {
                notches.add(Point(x,xy.y),Point(x,xy.y-5));
                x += dist;
            }
        }
        // label under the line
        label.move(length/3,xy.y+20);
        break;
    }
    case Axis::y:
    {
        Shape::add(xy); // a y-axis goes up
        Shape::add(Point(xy.x,xy.y-length));

        if (1<n) {      // add notches
            int dist = length/n;
            int y = xy.y-dist;
            for (int i = 0; i<n; ++i) {
                notches.add(Point(xy.x,y),Point(xy.x+5,y));
                y -= dist;
            }
        }
        // label at top
        label.move(xy.x-10,xy.y-length-10);
        break;
    }
    case Axis::z:
        error("z axis not implemented");
    }
}

//------------------------------------------------------------------------------

void Axis::draw_lines() const
{
    Shape::draw_lines();
    notches.draw();  // the notches may have a different color from the line
    label.draw();    // the label may have a different color from the line
}

//------------------------------------------------------------------------------

void Axis::set_color(Color c)
{
    Shape::set_color(c);
    notches.set_color(c);
    label.set_color(c);
}

//------------------------------------------------------------------------------

void Axis::move(int dx, int dy)
{
    Shape::move(dx,dy);
    notches.move(dx,dy);
    label.move(dx,dy);
}



//------------------------------------------------------------------------------

Function::Function(Fct f, double r1, double r2, Point xy,
                   int count, double xscale, double yscale)
// graph f(x) for x in [r1:r2) using count line segments with (0,0) displayed at xy
// x coordinates are scaled by xscale and y coordinates scaled by yscale
{
    if (r2-r1<=0) error("bad graphing range");
    if (count <=0) error("non-positive graphing count");
    double dist = (r2-r1)/count;
    double r = r1;
    for (int i = 0; i<count; ++i) {
        add(Point(xy.x+int(r*xscale),xy.y-int(f(r)*yscale)));
        r += dist;
    }
}

//------------------------------------------------------------------------------

bool can_open(const string& s)
// check if a file named s exists and can be opened for reading
{
    ifstream ff(s.c_str());
    return ff.operator bool();
}

//------------------------------------------------------------------------------

#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))

Suffix::Encoding get_encoding(const string& s)
{
    struct SuffixMap 
    { 
        const char*      extension;
        Suffix::Encoding suffix;
    };

    static SuffixMap smap[] = {
        {".jpg",  Suffix::jpg},
        {".jpeg", Suffix::jpg},
        {".gif",  Suffix::gif},
    };

    for (int i = 0, n = ARRAY_SIZE(smap); i < n; i++)
    {
        int len = strlen(smap[i].extension);

        if (s.length() >= len && s.substr(s.length()-len, len) == smap[i].extension)
            return smap[i].suffix;
    }

    return Suffix::none;
}

//------------------------------------------------------------------------------

// somewhat over-elaborate constructor
// because errors related to image files can be such a pain to debug
Image::Image(Point xy, string s, Suffix::Encoding e)
    :w(0), h(0), fn(xy,"")
{
    add(xy);

    if (!can_open(s)) {    // can we open s?
        fn.set_label("cannot open \""+s+'\"');
        p = new Bad_image(30,20);    // the "error image"
        return;
    }

    if (e == Suffix::none) e = get_encoding(s);

    switch(e) {        // check if it is a known encoding
    case Suffix::jpg:
        p = new Fl_JPEG_Image(s.c_str());
        break;
    case Suffix::gif:
        p = new Fl_GIF_Image(s.c_str());
        break;
    default:    // Unsupported image encoding
        fn.set_label("unsupported file type \""+s+'\"');
        p = new Bad_image(30,20);    // the "error image"
    }
}

//------------------------------------------------------------------------------

void Image::draw_lines() const
{
    if (fn.label()!="") fn.draw_lines();

    if (w&&h)
        p->draw(point(0).x,point(0).y,w,h,cx,cy);
    else
        p->draw(point(0).x,point(0).y);
}

//------------------------------------------------------------------------------

void Image::set_img(Point new_xy, int new_w, int new_h)
{
	set_point(0, Point(new_xy.x, new_xy.y));
	set_mask(Point(new_xy.x, new_xy.y), new_w, new_h);
}

//------------------------------------------------------------------------------
Arc::Arc(Point p, int ww, int hh, double a1, double a2)
:w(ww), h(hh),a1(a1),a2(a2)
{
	add(Point(p.x, p.y));
}
Point Arc::center() const
{
	return Point(point(0).x, point(0).y);
}
void Arc::draw_lines() const
{
	if (color().visibility()) fl_arc(point(0).x, point(0).y, w + w, h + h, a1, a2);
}


//------------------------------------------------------------------------------

Box::Box(Point pp, int aa, int bb, int rr)
:a(aa), b(bb), r(rr)
{
	ini(pp);
}
void Box::ini(Point p1)
{
	p1.y += r;
	Shape::add(p1);
	p1.y += b - 2 * r;
	Shape::add(p1);
	p1.x += r;
	p1.y += r;
	Shape::add(p1);
	p1.x += a - 2 * r;
	Shape::add(p1);
	p1.x += r;
	p1.y -= r;
	Shape::add(p1);
	p1.y -= b - 2 * r;
	Shape::add(p1);
	p1.y -= r;
	p1.x -= r;
	Shape::add(p1);
	p1.x -= a - 2 * r;
	Shape::add(p1);
	p1.y += r;
	Shape::add(p1);
	p1.y += b - 2 * r;
	Shape::add(p1);
	p1.x += a - 2 * r;
	Shape::add(p1);
	p1.y -= b - 2 * r;
	Shape::add(p1);
}
void Box::draw_lines() const
{
	if (color().visibility()) {
		fl_color(color().as_int());
		for (int i = 1; i <= 8; i += 2) {
			fl_line(point(i - 1).x, point(i - 1).y, point(i).x, point(i).y);
		}
		fl_arc(point(8).x - r, point(8).y - r, r + r, r + r, 90, 180);
		fl_arc(point(9).x - r, point(9).y - r, r + r, r + r, 180, 270);
		fl_arc(point(10).x - r, point(10).y - r, r + r, r + r, 270, 360);
		fl_arc(point(11).x - r, point(11).y - r, r + r, r + r, 0, 90);
	}
}


//------------------------------------------------------------------------------
Striped_Rectangle::Striped_Rectangle(Point xy, int ww, int hh, int gg, int dd)
: h(hh),w(ww),gap(gg),wid(dd)
{
	int temp = xy.y + hh;
	add(xy);
	if (dd / 2 > gap) error("line_width is too large");
	for (;xy.y < temp - gg;) {
		xy.y += gg;
		add(xy);
		xy.x += ww - 2;
		add(xy);
		xy.x -= ww - 2;
	}
	if (h <= 0 || w <= 0) error("Bad rectangle: non-positive side");
}

void Striped_Rectangle::draw_lines() const
{
	if (fill_color().visibility()) {    // fill
		fl_color(fill_color().as_int());
		for (int i = 1; i < number_of_points(); i += 2) {
			fl_rectf(point(i).x, point(i).y - wid / 2, w - 2, wid);
		}
	}
	if (color().visibility()) {    // lines on top of fill
		fl_color(color().as_int());
		fl_rect(point(0).x, point(0).y, w, h);
	}
}

//------------------------------------------------------------------------------

Binary_Tree::Binary_Tree(Point xy, int ll, int tt)
	: levels(ll), type(tt)
{
	//if (ll <= 0) error("Not enough levels");
	add(xy);
	for (int i = 2; i <= ll; i++) {
		int t = pow(2, ll - i + 5);
		int pstart = calpow(i - 2);
		xy.x = point(pstart).x - t;
		xy.y = point(pstart).y + t;
		add(xy);
		for (int j = 1; j < pow(2, i - 1); j++) {
			xy.x += 2 * t;
			add(xy);
		}
	}
	for (int i = 0; i < calpow(ll); i++) {
		haveword[i] = false;
	}
}

void Binary_Tree::draw_lines() const
{
	int num;
	if (color().visibility()) {
		fl_color(color().as_int());
		
		int t = 1;
		for (int i = 1; i < levels; i++) {
			num = calpow(i);
			if (type == 1) {
				for (int j = calpow(i - 1); j < calpow(i); j++) {
					fl_line(point(j).x, point(j).y + 4, point(num).x, point(num).y - 4);
					fl_line(point(j).x, point(j).y + 4, point(num + 1).x, point(num + 1).y - 4);
					num += 2;
				}
			}
			else if (type == 2) {
				for (int j = calpow(i - 1); j < calpow(i); j++) {
					fl_line(point(j).x, point(j).y + 4, point(num).x, point(num).y - 4);
					fl_line(point(num).x, point(num).y - 4, point(num).x + 12, point(num).y - 20);
					fl_line(point(num).x, point(num).y - 4, point(num).x + 14, point(num).y - 10);
					fl_line(point(j).x, point(j).y + 4, point(num + 1).x, point(num + 1).y - 4);
					fl_line(point(num + 1).x, point(num + 1).y - 4, point(num + 1).x - 12, point(num + 1).y - 20);
					fl_line(point(num + 1).x, point(num + 1).y - 4, point(num + 1).x - 14, point(num + 1).y - 10);
					num += 2;
				}
			}
		}
		for (int i = 0; i < number_of_points(); i++) {
			if (haveword[i]) {
				fl_font(Font::times_bold, 20);
				fl_draw(word[i].c_str(), point(i).x - 5, point(i).y);
			}
			else fl_arc(point(i).x - 5, point(i).y - 5, 10, 10, 0, 360);
		}
	}
}

void Binary_Tree::add_text(string s, string move) {
	int change = 0;
	for (int i = 0; i < levels && i < move.length(); i++) {
		switch (move[i]) {
		case 'L': change += change + 1; break;
		case 'R': change += change + 2; break;
		}
	}
	word[change] = s;
	haveword[change] = true;
	
}

//------------------------------------------------------------------------------

Clock::Clock(Point(xy), int r)
	:radius(r)
{
	get_time();
	add(xy);
	xy.y -= r;
	add(xy);
	xy.x += r;
	xy.y += r;
	add(xy);
	xy.x -= r;
	xy.y += r;
	add(xy);
	xy.x -= r;
	xy.y -= r;
	add(xy);
}

void Clock::get_time()
{
	time_t t = time(0);
	struct tm *p;
	p = localtime(&t);
	currh = p->tm_hour;
	currm = p->tm_min;
	currs = p->tm_sec;
	stringstream ss;
	
	if (currh >= 10) ss << currh;
	else ss << "0" << currh;
	if (currm >= 10) ss << ":" << currm;
	else ss << ":0" << currm;
	if (currs >= 10) ss << ":" << currs;
	else ss << ":0" << currs;
	time_now = ss.str();
}

void Clock::draw_lines() const
{

	if (color().visibility())
	{
		fl_circle(point(0).x, point(0).y, radius);
		fl_line(point(1).x, point(1).y, point(1).x, point(1).y + 25);
		fl_line(point(2).x, point(2).y, point(2).x - 25, point(2).y);
		fl_line(point(3).x, point(3).y, point(3).x, point(3).y - 25);
		fl_line(point(4).x, point(4).y, point(4).x + 25, point(4).y);
		fl_font(Font::times_bold, 30);
		fl_draw(time_now.c_str(), 195, 120);
		int ang1;
		if (currs < 15) ang1 = -6 * currs + 90;
		else ang1 = -6 * currs + 450;
		for (int i = 10; i <= 100; i+=5)
		{
			fl_arc(point(0).x - (radius - i), point(0).y - (radius - i), 2 * radius - 2 * i, 2 * radius - 2 * i, ang1 - 5, ang1 + 5);
		}
		if (currm < 15) ang1 = -6 * currm + 90;
		else ang1 = -6 * currm + 450;
		for (int i = 30; i <= 100; i += 5)
		{
			fl_arc(point(0).x - (radius - i), point(0).y - (radius - i), 2 * radius - 2 * i, 2 * radius - 2 * i, ang1 - 4, ang1 + 4);
		}
		if (currh < 15) ang1 = -30 * currh + 90;
		else ang1 = -30 * currh + 450;
		for (int i = 50; i <= 100; i += 5)
		{
			fl_arc(point(0).x - (radius - i), point(0).y - (radius - i), 2 * radius - 2 * i, 2 * radius - 2 * i, ang1 - 2, ang1 + 2);
		}
	}
	
}
//------------------------------------------------------------------------------

Airplane::Airplane(Point xy)
{
	add(xy);
	xy.x += 60;
	add(xy);
	xy.x -= 40;
	add(xy);
	xy.x += 20;
	xy.y -= 30;
	add(xy);
	xy.y += 60;
	add(xy);
	xy.x = point(1).x + 10;
	xy.y -= 40;
	add(xy);
	xy.y += 20;
	add(xy);
}

void Airplane::draw_lines() const
{
	if (color().visibility()) {
		fl_line(point(0).x, point(0).y, point(1).x, point(1).y);
		fl_line(point(1).x, point(1).y, point(5).x, point(5).y);
		fl_line(point(1).x, point(1).y, point(6).x, point(6).y);
		fl_line(point(2).x, point(2).y, point(3).x, point(3).y);
		fl_line(point(2).x, point(2).y, point(4).x, point(4).y);
	}
}

Time_Display::Time_Display(Point(xy))
{
	get_time();
	add(xy);
}

void Time_Display::get_time()
{
	time_t t = time(0);
	struct tm *p;
	p = localtime(&t);
	currh = p->tm_hour;
	currm = p->tm_min;
	currs = p->tm_sec;
	stringstream ss;

	if (currh >= 10) ss << currh;
	else ss << "0" << currh;
	if (currm >= 10) ss << ":" << currm;
	else ss << ":0" << currm;
	//if (currs >= 10) ss << ":" << currs;
	//else ss << ":0" << currs;
	time_now = ss.str();
}

void Time_Display::draw_lines() const
{
	if (color().visibility()) {
		fl_font(Font::helvetica, 55);
		fl_draw(time_now.c_str(), point(0).x, point(0).y);
	}
}

Time_Display_2::Time_Display_2(Point(xy))
{
	get_time();
	add(xy);
}

void Time_Display_2::get_time()
{
	time_t t = time(0);
	struct tm *p;
	p = localtime(&t);
	currh = p->tm_hour;
	currm = p->tm_min;
	currs = p->tm_sec;
	stringstream ss;

	if (currh >= 10) ss << currh;
	else ss << "0" << currh;
	if (currm >= 10) ss << ":" << currm;
	else ss << ":0" << currm;
	//if (currs >= 10) ss << ":" << currs;
	//else ss << ":0" << currs;
	time_now = ss.str();
}

void Time_Display_2::draw_lines() const
{
	if (color().visibility()) {
		fl_font(Font::helvetica, 15);
		fl_draw(time_now.c_str(), point(0).x, point(0).y);
	}
}
//------------------------------------------------------------------------------

} // of namespace Graph_lib
