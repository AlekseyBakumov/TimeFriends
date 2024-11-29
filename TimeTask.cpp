#include <iostream>
class SimpleWatch;
class Time;

class Watch
{
public:
    Watch() { std::cout << "Called Watch constructor." << std::endl; }
    ~Watch() { std::cout << "Called Watch destructor." << std::endl; }
    void PrintTime(const Time& time, bool format_24h) noexcept;
    void SetTime(Time* time, int hours, int minutes, int seconds, bool format_24h);
};

class Time
{
    int hours, minutes, seconds;
    static unsigned int counter;

public:
    Time() noexcept
    {
        hours = 0;
        minutes = 0;
        seconds = 0;
        counter++;
        std::cout << "Time object created using default constructor. Current counter: " << counter << std::endl;
    }
    Time(int h, int m, int s) : hours(h), minutes(m), seconds(s)
    {
        if (h < 0 || h > 24 ||
            m < 0 || m > 60 ||
            s < 0 || s > 60)
            throw std::invalid_argument("Invalid argument for constructor");
        Normalize();
        counter++;
        std::cout << "Time object created using initializing constructor. Current counter: " << counter << std::endl;
    }
    Time(const Time& t) noexcept : hours(t.GetHours()), minutes(t.GetMinutes()), seconds(t.GetSeconds())
    {
        Normalize();
        counter++;
        std::cout << "Time object created using copy constructor. Current counter: " << counter << std::endl;
    }
    ~Time() noexcept
    { 
        counter--;
        std::cout << "Time object destroyed. Current counter: " << counter << std::endl;
    }

    Time& operator= (const Time& other) noexcept {
        hours = other.hours;
        minutes = other.minutes;
        seconds = other.seconds;
        return *this;
    }

    int GetHours() const { return hours; }
    int GetMinutes() const { return minutes; }
    int GetSeconds() const { return seconds; }

    void SetHours(int h)
    {
        if (h < 0 || h > 24)
            throw std::invalid_argument("Invalid argument for set function");
        hours = h;

    }
    void SetMinutes(int m)
    {
        if (m < 0 || m > 60)
            throw std::invalid_argument("Invalid argument for set function");
        minutes = m;

    }
    void SetSeconds(int s)
    {
        if (s < 0 || s > 60)
            throw std::invalid_argument("Invalid argument for set function");
        seconds = s;

    }

    void PrintTime() noexcept {
        std::cout 
            << "H:" << this->GetHours()
            << " M:" << this->GetMinutes()
            << " S:" << this->GetSeconds()
            << std::endl;
    }

    static int GetCount() { return counter; }

    void Normalize() noexcept {
        minutes += seconds / 60;
        seconds %= 60;
        hours += minutes / 60;
        minutes %= 60;
        hours %= 24;

        if (seconds < 0)
        {
            seconds += 60;
            minutes--;
        }

        if (minutes < 0)
        {
            minutes += 60;
            hours--;
        }

        if (hours < 0)
        {
            hours += 24;
        }
    }

    int ToSeconds() { return ((GetHours() * 60) + GetMinutes()) * 60 + GetSeconds(); }

    Time operator+ (int s) noexcept
    {
        return Time(GetHours(), GetMinutes(), GetSeconds() + s);
    }
    Time operator+= (int s) noexcept
    {
        seconds += s;
        Normalize();
        return *this;
    }
    Time operator- (int s) noexcept
    {
        return Time(GetHours(), GetMinutes(), GetSeconds() - s);
    }
    Time operator-= (int s) noexcept
    {
        seconds -= s;
        Normalize();
        return *this;
    }

    bool operator== (const Time& t) noexcept
    {
        return (GetSeconds() == t.GetSeconds()
            && GetMinutes() == t.GetMinutes()
            && GetSeconds() == t.GetSeconds());
    }
    bool operator!= (const Time& t) noexcept
    {
        return !(*this == t);
    }
    bool operator< (const Time& t) noexcept
    {
        if (GetHours() < t.GetHours()) return true;
        if (GetHours() > t.GetHours()) return false;
        if (GetMinutes() < t.GetMinutes()) return true;
        if (GetMinutes() > t.GetMinutes()) return false;
        if (GetSeconds() < t.GetSeconds()) return true;
        return false;
    }
    bool operator> (const Time& t) noexcept
    {
        return (!(*this < t)) && (*this != t);
    }
    bool operator<= (const Time& t) noexcept
    {
        return !(*this>t);
    }
    bool operator>= (const Time& t) noexcept
    {
        return !(*this < t);
    }

    friend class SimpleWatch;
    friend void Watch::PrintTime(const Time& time, bool format_24h) noexcept;
    friend void Watch::SetTime(Time* time, int hours, int minutes, int seconds, bool format_24h);
};
unsigned int Time::counter = 0;

void Watch::PrintTime(const Time& time, bool format_24h=true) noexcept
{
    std::cout
        << "H:" << (format_24h ? time.hours : (time.hours % 12))
        << " M:" << time.minutes
        << " S:" << time.seconds;
    if (!format_24h)
        std::cout << (time.hours / 12 == 1 ? "PM" : "AM");
    std::cout << std::endl;
}
void Watch::SetTime(Time* time, int hours, int minutes, int seconds, bool format_24h=true)
{
    time->hours = hours;
    time->minutes = minutes;
    time->seconds = seconds;
}

class SimpleWatch 
{
    bool format_24h = true;
public:
    void SetFormat24h(bool flag) { format_24h = flag; }
    void PrintTime(const Time& time) noexcept
    {
        std::cout
            << "H:" << (format_24h ? time.hours : (time.hours % 12))
            << " M:" << time.minutes
            << " S:" << time.seconds;
        if (!format_24h)
            std::cout << (time.hours / 12 == 1 ? " PM" : " AM");
        std::cout << std::endl;
    }

    void SetTime(Time* time, int hours, int minutes, int seconds)
    {
        time->hours = hours;
        time->minutes = minutes;
        time->seconds = seconds;
    }
};

class WallClock : public Watch
{
protected:
    Time time;
public:
    WallClock (int hours, int minutes, int seconds) : time(hours, minutes, seconds) { std::cout << "Called WallClock constructor." << std::endl; }
    ~WallClock() { std::cout << "Called WallClock destructor." << std::endl; }

    void CheckTime() { PrintTime(time); }
};

class CuckooClock : public WallClock
{
public:
    CuckooClock(int hours, int minutes, int seconds) : WallClock(hours, minutes, seconds) { std::cout << "Called CuckooClock constructor." << std::endl; }
    ~CuckooClock() { std::cout << "Called CuckooClock destructor." << std::endl; }

    void HourPassed()
    {
        std::cout << "Clock on the wall: ";
        for (int i = time.GetHours(); i > 0; i--)
            std::cout << "Cuckoo! ";
        std::cout << std::endl;
    }
};

class HandWatch : public Watch
{
protected:
    Time time;
public:
    HandWatch(int hours, int minutes, int seconds) : time(hours, minutes, seconds) { std::cout << "Called HandWatch constructor." << std::endl; }
    ~HandWatch() { std::cout << "Called HandWatch destructor." << std::endl; }

    void CheckTime() { PrintTime(time); }
};

int main()
{
    Time t;
    t.SetHours(1);
    Time t2 = t;
    t2.SetHours(2);
    t2.PrintTime();
    {
        t2.SetHours(12);
        t2.PrintTime();
        Time t2(22, 22, 22);
        t2.PrintTime();
    }
    t2.PrintTime();
    Time t3(3, 3, 3);
    t3 = t;
    t3.SetHours(3);
    t3.PrintTime();

    std::cout << std::endl;
    std::cout << "***====Examples with *new====***" << std::endl;
    {
        Time* nt1 = new Time();
        Time* nt2;
        std::cout << "Created variable pointer" << std::endl;
        nt2 = new Time(*nt1);

        {
            Time* nt3 = new Time(4, 0, 0);
            nt3->SetSeconds(20);
            delete nt3;
        }
        delete nt1;

        nt1 = nt2;
        delete nt1;
        nt1 = nullptr;
        nt2 = nullptr;
    }

    std::cout << std::endl;
    std::cout << "***====Friends====***" << std::endl;
    {
        SimpleWatch sw;
        sw.SetTime(&t3, 5, 5, 10);
        sw.PrintTime(t3);

        Watch w;
        w.SetTime(&t3, 20, 12, 9);
        sw.SetFormat24h(false);
        sw.PrintTime(t3);
        sw.SetTime(&t3, 1, 1, 1);
        w.PrintTime(t3);
    }

    std::cout << std::endl;
    std::cout << "***====Exceptions====***" << std::endl;
    {
        try
        {
            t3.SetMinutes(-100);
        }
        catch (...)
        {
            std::cout << "Negative value exception" << std::endl;
        }

        try
        {
            t3.SetSeconds(10000);
        }
        catch (std::invalid_argument exc)
        {
            std::cout << "Exception occured: " << exc.what() << std::endl;
        }

        //Time t4(-100, 10000, 0);
    }

    std::cout << std::endl;
    std::cout << "***====Inheritance====***" << std::endl;
    {
        CuckooClock wall_clock_c(3, 20, 14);
        wall_clock_c.CheckTime();
        wall_clock_c.HourPassed();
    }

    std::cout << std::endl;
    std::cout << "***====End of programm====***" << std::endl;

    return 0;
}
