//
// Created by PCAditiaID29 on 2/10/2021.
//

#ifndef RCHEATS_HACKSDRAWING_H
#define RCHEATS_HACKSDRAWING_H

#include <chrono>

Request request;
Response response;


void DrawingSkeleton(CanvasEngine esp, Vector2 vec1, Vector2 vec2, Color color){
    esp.DrawLine(color, 2, Vector2(vec1.X, vec1.Y), Vector2(vec2.X, vec2.Y));
}

bool isnull(Vector2 dat)
{
    if (dat.X != NULL)
        if (dat.Y != NULL)
            return true;
    return false;
}


uint64_t GetTickCount() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

class Interval {
private:
    int initial_;

public:
    inline Interval() : initial_(GetTickCount()) {}

    virtual ~Interval() {}

    inline unsigned int value() const {
        return GetTickCount() - initial_;
    }
};

class FPS {
protected:
    int32_t m_fps;
    int32_t m_fpscount;
    Interval m_fpsinterval;

public:
    FPS() : m_fps(0), m_fpscount(0) {}

    void Update() {
        m_fpscount++;
        if (m_fpsinterval.value() > 1000) {
            m_fps = m_fpscount;
            m_fpscount = 0;
            m_fpsinterval = Interval();
        }
    }

    int32_t get() const {
        return m_fps;
    }
};

FPS m_fps;


Color teamIdColors[] =
        {
                Color::Red(),
                Color::Yellow(),
                Color::Orange(),
                Color::BlueDongker(),
                Color::Pink(),
                Color::Green(),
                Color::Grey(),
                Color::SilverDark(),
                Color::Blue()
        };

Color colorByDistance(int distance, float alpha){
    Color _colorByDistance;
    if (distance < 450)
        _colorByDistance = Color(0,255,0, alpha);
    if (distance < 200)
        _colorByDistance = Color(255,255,0, alpha);
    if (distance < 120)
        _colorByDistance = Color(255,133,51, alpha);
    if (distance < 50)
        _colorByDistance = Color(255,0,0, alpha);
    return _colorByDistance;
}

extern "C"
void MainDraw(CanvasEngine rViewEngine){
    request.otherFeature.LessRecoil = isLessRecoil;
    request.otherFeature.InstantHit = isInstantHit;
    request.otherFeature.FastParachute = isFastParachute;
    send((void *) &request, sizeof(request));
    receive((void *) &response);
    int screenHeight = rViewEngine.getHeight();
    int screenWidth = rViewEngine.getWidth();
    m_fps.Update();
    char *tsr = OBFUSCATE("RCheats FPS : ");
    std::string Str = std::string(tsr);
    Str += std::to_string((int) m_fps.get());
    rViewEngine.DrawText(Color::Yellow(), Str.c_str(), Vector2(150, 50), 22);
    if (response.Success) {
        int count = response.PlayerCount;
        if (count > 0){
            for (int i = 0; i < count; i++) {
                PlayerData player = response.Players[i];
                float x = player.HeadLocation.X;
                float y = player.HeadLocation.Y;
                float z = player.Location.Z;
                float h = z / 2;
                if (player.Distance < 5 || player.Distance > 450) continue;
                string sDistance;
                sDistance += to_string((int) player.Distance);
                sDistance += "M";
                Color _colorByDistance = colorByDistance((int)player.Distance, 255);
                if (showName || showHealth || showTeamID) {
                    Color color = Color(0, 3, 12, 180);
                    rViewEngine.DrawLine(color, 30, Vector2(x - player.HealthMax, y - 60), Vector2(x + player.HealthMax, y - 60));
                }
                if (showBox){
                    rViewEngine.DrawRect(_colorByDistance, 2, Vector2(player.Bone.pelvis.X - h / 4 - 7, player.HeadLocation.Y - 14), Vector2(player.Bone.pelvis.X + h / 4 + 7, player.Bone.rKnee.Y + 14));
                }
                if (showDistance){
                    float a = 20;
                    if (player.Health >= 100) {
                        a = 26;
                    }
                    rViewEngine.DrawText(Color::Orange2(), sDistance.c_str(), Vector2(x + player.HealthMax - a, y - 53), 15);
                }
                if (showTeamID) {
                    Color textColor = Color::White();
                    Color tColors;
                    if (player.TeamID < 100)
                    {
                        textColor = Color::White();
                        tColors = teamIdColors[0];
                    }
                    if (player.TeamID < 95)
                    {
                        textColor = Color(0, 3, 12, 180);
                        tColors = teamIdColors[1];
                    }
                    if (player.TeamID < 90)
                    {
                        textColor = Color(0, 3, 12, 180);
                        tColors = teamIdColors[2];
                    }
                    if (player.TeamID < 85)
                    {
                        textColor = Color::White();
                        tColors = teamIdColors[3];
                    }
                    if (player.TeamID < 80)
                    {
                        textColor = Color::White();
                        tColors = teamIdColors[4];
                    }
                    if (player.TeamID < 75)
                    {
                        textColor = Color::White();
                        tColors = teamIdColors[5];
                    }
                    if (player.TeamID < 70)
                    {
                        textColor = Color::White();
                        tColors = teamIdColors[6];
                    }
                    if (player.TeamID < 65)
                    {
                        textColor = Color::White();
                        tColors = teamIdColors[7];
                    }
                    if (player.TeamID < 60)
                    {
                        textColor = Color::White();
                        tColors = teamIdColors[8];
                    }
                    if (player.TeamID < 55)
                    {
                        textColor = Color::White();
                        tColors = teamIdColors[0];
                    }
                    if (player.TeamID < 50)
                    {
                        textColor = Color(0, 3, 12, 180);
                        tColors = teamIdColors[1];
                    }
                    if (player.TeamID < 45)
                    {
                        textColor = Color(0, 3, 12, 180);
                        tColors = teamIdColors[2];
                    }
                    if (player.TeamID < 40)
                    {
                        textColor = Color::White();
                        tColors = teamIdColors[3];
                    }
                    if (player.TeamID < 35)
                    {
                        textColor = Color::White();
                        tColors = teamIdColors[4];
                    }
                    if (player.TeamID < 30)
                    {
                        textColor = Color::White();
                        tColors = teamIdColors[5];
                    }
                    if (player.TeamID < 25)
                    {
                        textColor = Color::White();
                        tColors = teamIdColors[6];
                    }
                    if (player.TeamID < 20)
                    {
                        textColor = Color::White();
                        tColors = teamIdColors[7];
                    }
                    if (player.TeamID < 15)
                    {
                        textColor = Color::White();
                        tColors = teamIdColors[8];
                    }
                    if (player.TeamID < 10)
                    {
                        textColor = Color::White();
                        tColors = teamIdColors[0];
                    }
                    if (player.TeamID < 5)
                    {
                        textColor = Color(0, 3, 12, 180);
                        tColors = teamIdColors[1];
                    }
                    std::string sTeamID;
                    float a = 20;
                    if (player.Health >= 100) {
                        a = 26;
                    }
                    sTeamID += to_string((int) player.TeamID);
                    rViewEngine.DrawLine(tColors, 30, Vector2(x - player.HealthMax , y - 60), Vector2(x - player.HealthMax + 50, y - 60));
                    rViewEngine.DrawText2(textColor, sTeamID.c_str(), Vector2(x - player.HealthMax + a, y - 53), 15);
                }
                if (showName){
                    if (!player.IsRobot){
                        rViewEngine.DrawText2(Color::White(), player.Name, Vector2(x, y - 53), 15);
                    }
                }
                if (showRadarLine){
                    float t = 0;
                    float at = 0;
                    if (showName){
                        t = 75;
                    }
                    if (showEnemyCount){
                        at = 125;
                    }
                    rViewEngine.DrawLine(_colorByDistance, 2, Vector2(rViewEngine.getWidth() / 2, at), Vector2(x, y - t));
                }
                if (showHealth) {
                    Color healthColor = Color::White();
                    Vector2 v = Vector2(x + (-player.HealthMax) +
                                        2 * player.HealthMax * player.Health / player.HealthMax,
                                        y - 40);
                    if ((int)player.Health == 0) {
                        healthColor = Color::Red();
                        v = Vector2(x + player.HealthMax, y - 40);
                        rViewEngine.DrawText(Color::Orange3(), "Knocked",
                                             Vector2(player.Bone.rKnee.X, player.Bone.rKnee.Y + 50), 15);
                    }
                    rViewEngine.DrawLine(healthColor, 10, Vector2(x - player.HealthMax, y - 40), v);
                }
                if (showSkeleton) {
                    Color _skeletonColors = colorByDistance((int)player.Distance, 255);
                    if (isnull(player.HeadLocation) && isnull(player.Bone.chest) &&
                        isnull(player.Bone.pelvis) && isnull(player.Bone.lSholder)
                        && isnull(player.Bone.rSholder) && isnull(player.Bone.lElbow) &&
                        isnull(player.Bone.rElbow) && isnull(player.Bone.lWrist)
                        && isnull(player.Bone.rWrist) && isnull(player.Bone.lThigh) &&
                        isnull(player.Bone.rThigh) && isnull(player.Bone.lKnee)
                        && isnull(player.Bone.rKnee) && isnull(player.Bone.lAnkle) &&
                        isnull(player.Bone.rAnkle)) {
                        DrawingSkeleton(rViewEngine, player.Bone.neck, player.Bone.chest, _skeletonColors);
                        DrawingSkeleton(rViewEngine, player.Bone.chest, player.Bone.pelvis, _skeletonColors);

                        DrawingSkeleton(rViewEngine, player.Bone.chest, player.Bone.lSholder, _skeletonColors);
                        DrawingSkeleton(rViewEngine, player.Bone.chest, player.Bone.rSholder, _skeletonColors);

                        DrawingSkeleton(rViewEngine, player.Bone.lSholder, player.Bone.lElbow, _skeletonColors);
                        DrawingSkeleton(rViewEngine, player.Bone.rSholder, player.Bone.rElbow, _skeletonColors);

                        DrawingSkeleton(rViewEngine, player.Bone.lElbow, player.Bone.lWrist, _skeletonColors);
                        DrawingSkeleton(rViewEngine, player.Bone.rElbow, player.Bone.rWrist, _skeletonColors);

                        DrawingSkeleton(rViewEngine, player.Bone.pelvis, player.Bone.lThigh, _skeletonColors);
                        DrawingSkeleton(rViewEngine, player.Bone.pelvis, player.Bone.rThigh, _skeletonColors);

                        DrawingSkeleton(rViewEngine, player.Bone.lThigh, player.Bone.lKnee, _skeletonColors);
                        DrawingSkeleton(rViewEngine, player.Bone.rThigh, player.Bone.rKnee, _skeletonColors);

                        DrawingSkeleton(rViewEngine, player.Bone.lKnee, player.Bone.lAnkle, _skeletonColors);
                        DrawingSkeleton(rViewEngine, player.Bone.rKnee, player.Bone.rAnkle, _skeletonColors);
                    }
                }
                if (showHeadDots){
                    Color _HeadDotsColors = colorByDistance((int)player.Distance, 255);
                    rViewEngine.DrawCircle(_HeadDotsColors, Vector2(x, y), z / 12);
                }
                if (showWeapon) {
                    float a = 20;
                    if (player.Health >= 100) {
                        a = 26;
                    }
                    if (player.Weapon1.WeaponId > 0) {
                        rViewEngine.DrawEnemyWeapon(Color::White(), player.Weapon1.WeaponId,
                                                    player.Weapon1.CurBulletNumInClip,
                                                    Vector2(x - player.HealthMax + a, y - 83), 15);
                    }
                    if (player.Weapon2.WeaponId > 0) {
                        rViewEngine.DrawEnemyWeapon(Color::White(), player.Weapon2.WeaponId,
                                                    player.Weapon2.CurBulletNumInClip,
                                                    Vector2(x + player.HealthMax - 20, y - 83), 15);
                    }
                }
                if (show360Warning) {
                    Color _360warningColor = colorByDistance((int)player.Distance, 128);
                    float locZ = player.Location.Z;
                    float posX = x;
                    float posY = y;
                    float radius = screenHeight / 15;
                    if (locZ == 1.0f) {
                        if (posY > screenHeight - screenHeight / 12)
                            posY = screenHeight - screenHeight / 12;
                        else if (posY < screenHeight / 12)
                            posY = screenHeight / 12;
                        if (posX < screenWidth / 2) {
                            rViewEngine.DrawCircle(_360warningColor, Vector2(screenWidth, posY), radius);
                            rViewEngine.DrawText2(Color::White(), sDistance.c_str(), Vector2(screenWidth - screenWidth / 65, posY), 15);
                        } else {
                            rViewEngine.DrawCircle(_360warningColor, Vector2(0, posY), radius);
                            rViewEngine.DrawText2(Color::White(), sDistance.c_str(), Vector2(screenWidth / 65, posY), 15);
                        }
                    }
                    else if (posX < -screenWidth / 10 || posX > screenWidth + screenWidth / 10) {
                        if (posY > screenHeight - screenHeight / 12)
                            posY = screenHeight - screenHeight / 12;
                        else if (posY < screenHeight / 12)
                            posY = screenHeight / 12;
                        if (posX > screenWidth / 2) {
                            rViewEngine.DrawCircle(_360warningColor, Vector2(screenWidth, posY), radius);
                            rViewEngine.DrawText2(Color::White(), sDistance.c_str(), Vector2(screenWidth - screenWidth / 65, posY), 15);
                        } else {
                            rViewEngine.DrawCircle(_360warningColor, Vector2(0, posY), radius);
                            rViewEngine.DrawText2(Color::White(), sDistance.c_str(), Vector2(screenWidth / 65, posY), 15);
                        }
                    }
                }
            }
        }
        if (showEnemyCount){
            int colors;
            std::string eCountStr;
            if (count != 0){
                colors = 2;
                if (count > 5)
                    colors = 2;
                if (count > 10)
                    colors = 3;
                if (count > 15)
                    colors = 4;
                eCountStr = std::to_string((int) count);
            } else {
                colors = 1;
                eCountStr = "CLEAR";
            }
            rViewEngine.DrawEnemyCount(colors, Vector2(rViewEngine.getWidth()/2, 20));
            rViewEngine.DrawText2(Color::Black(), eCountStr.c_str(), Vector2(rViewEngine.getWidth() / 2, 110), 25);
        }
        int grenadeCount = response.GrenadeCount;
        if (grenadeCount > 0){
            for (int i = 0; i < grenadeCount; i++) {
                if (showGrenadeAlert) {
                    if (response.Grenade[i].Distance < 0 || response.Grenade[i].Distance > 200) continue;
                    rViewEngine.DrawText(Color(255, 0, 0), "[ALERT] Granade!", Vector2(screenWidth / 2, 150), 15);
                    if (response.Grenade[i].Location.Z != 1.0f) {
                        char grenadeAlert[100];
                        if (response.Grenade[i].Type == 1)
                            sprintf(grenadeAlert, "[ALERT] Smoke Granade! (%dM)",
                                    (int) response.Grenade[i].Distance);
                        if (response.Grenade[i].Type == 2)
                            sprintf(grenadeAlert, "[ALERT] Burn Granade! (%dM)",
                                    (int) response.Grenade[i].Distance);
                        if (response.Grenade[i].Type == 3)
                            sprintf(grenadeAlert, "[ALERT] Flash Granade! (%dM)",
                                    (int) response.Grenade[i].Distance);
                        if (response.Grenade[i].Type == 4)
                            sprintf(grenadeAlert, "[ALERT] Frag Granade! (%dM)",
                                    (int) response.Grenade[i].Distance);
                        rViewEngine.DrawText(Color(255, 0, 0), grenadeAlert,
                                             Vector2(response.Grenade[i].Location.X,
                                                     response.Grenade[i].Location.Y), 15);
                    }
                }
            }
        }
    }
}

#endif //RCHEATS_HACKSDRAWING_H
