// #include stdio.h
#include <ROOT/REveScene.hxx>
#include <ROOT/REveManager.hxx>
#include <ROOT/REvePointSet.hxx>
#include <ROOT/REveTrack.hxx>

using namespace ROOT::Experimental;

void reproduce(char c, std::string& out)
{
    static const std::string rep = "slsrsls";
    if (c == 's')
       out += rep;
    else
      out += c;
}

REvePointSet *createPointSet(int npoints = 2, float s = 2, int color = kMagenta)
{
   TRandom &r = *gRandom;

   REvePointSet *ps = new REvePointSet("MyTestPoints", "list of eve points", npoints);

   for (Int_t i=0; i < npoints; ++i)
      ps->SetNextPoint(r.Uniform(-s,s), r.Uniform(-s,s), r.Uniform(-s,s));

   ps->SetMarkerColor(color);
   ps->SetMarkerSize(5 + r.Uniform(1, 15));
   ps->SetMarkerStyle(4);
   return ps;
}
void koch()
{
    // make pattern
    std::string path = "s";
    int maxgen = 7;
    for (int i = 0; i < maxgen; ++i)
    {
        std::cout << "path << " << path << "\n";
        std::string newpath;
        for (auto& it : path) {
            reproduce (it, newpath);
        }
        path = newpath;
    }

    // make trace
   auto eveMng = REveManager::Create();
    auto track = new REveLine("Koch");
    float x = 0, y = 0;
    float ang = 0;
    float stepSize = 1;
    for (auto& it : path) {
        char c = it;
        switch (c) {
            case 's':
                x += stepSize * cos(ang);
                y += stepSize * sin(ang);
                track->SetNextPoint(x, y, 0);
                break;
            case 'r':
                ang -= TMath::TwoPi()/3;
                break;
            case 'l':
                ang += TMath::Pi()/3;
        };

    }

   eveMng->GetEventScene()->AddElement(track);


   auto ps = createPointSet(10, 1);
   eveMng->GetEventScene()->AddElement(ps);

    // start server
  //auto vl = (REveViewer*)eveMng->GetViewers()->FirstChild();
 //vl->SetCameraType(REveViewer::kCameraOrthoXOY);
   eveMng->Show();


}
