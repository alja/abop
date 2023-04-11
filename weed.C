// #include stdio.h
#include <ROOT/REveScene.hxx>
#include <ROOT/REveManager.hxx>
#include <ROOT/REvePointSet.hxx>
#include <ROOT/REveTrack.hxx>
#include <ROOT/REveStraightLineSet.hxx>

using namespace ROOT::Experimental;

void reproduce(char c, std::string& out)
{
    if (c == 'B')
       out += "F[+B]F[-B]+B";
    else if (c == 'F')
       out += "FF";
    else
      out += c;
}

struct LState
{
    float x{0};
    float y{0};
    float ang{0};
 LState(){};
    LState(float ix, float iy, float iang) :x(ix), y(iy), ang(iang){};

    void dump(std::string action) {std::cout << action << ": x = " << x << ", y =" << y << ", ang = " << ang << "\n";};
};

void printStack(std::vector<LState>& wstack)
{
    for (auto& i : wstack) {
        i.dump(">>>>> ");
    }
}

void weed(int maxgen = 5)
{
    // make pattern
    std::string path = "B";
    for (int i = 0; i < maxgen; ++i)
    {
        std::string newpath;
        for (auto& it : path) {
            reproduce (it, newpath);
        }
        path = newpath;
    }
    std::cout << "end path << " << path << "\n";

    // make trace
    float stepSize = 1;
    auto eveMng = REveManager::Create();
    auto lineset = new REveStraightLineSet("Weed");
    std::vector<LState> wstack;
    
    wstack.push_back(LState());
    LState w = wstack.back();
    LState apex = w;

    for (auto& it : path) {
        char c = it;
        std::cout << " __" << c << "___\n";
        switch (c) {
            case 'F':
                w.x += stepSize * cos(w.ang);
                w.y += stepSize * sin(w.ang);
                w.dump("step ");
                lineset->AddLine(apex.x, apex.y, 0, w.x, w.y, 0);
                apex = w;
                break;
            case '-':
                w.ang -= TMath::Pi()/6;
                break;
            case '+':
                w.ang += TMath::Pi()/6;
                break;
            case ']':
                w = wstack.back();
                wstack.pop_back();
                w.dump("afterpop");
                apex = w;
                printStack(wstack);
                break;
            case '[':
                wstack.push_back(LState(w.x, w.y, w.ang));
                w.dump("push");
                printStack(wstack);
                break;
            default:
                // std::cout <<"case error amt\n";
                break;
        };
    }
    REveViewer *v = (REveViewer *)eveMng->GetViewers()->FirstChild();
    v->SetCameraType(REveViewer::kCameraOrthoXOY);
    lineset->InitMainTrans();
    lineset->RefMainTrans().RotateLF(1, 2, 3.14 / 2);
    eveMng->GetEventScene()->AddElement(lineset);
    eveMng->Show();
}
