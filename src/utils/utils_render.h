#pragma once

class Baddy1Shape
{
public:
    void assign_item(ITEM_INFO* item);
    void assign_object(short objNumber);
    void assign_object_second(short objNumber);
    void create_interpolate();
    void create_shadow();
    void create_rotation();
    void set_world();
    void set_clipping();
    void set_light();
    void assign_bone();
    void set_list();
    void assign_rotation();
    void add_mesh(RENDER human);
    void end_list();
    void end_world();

    void add_flash(BITE_INFO* bite, bool popEnd);
    void baddy_uzi_holsters(short boneID, short meshID, short weaponID);
    void baddy_sword_holsters(short boneID, short meshID, short weaponID);
    void baddy_uzi(short boneID, short meshID, short weaponID);
    void baddy_sword(short boneID, short meshID, short weaponID);
    void baddy_sword1(short boneID, short meshID, short weaponID);
    void baddy_sword2(short boneID, short meshID, short weaponID);
    void baddy_sword3(short boneID, short meshID, short weaponID);
    void baddy_sword4(short boneID, short meshID, short weaponID);
    void baddy_sword5(short boneID, short meshID, short weaponID);
    void baddy_sword6(short boneID, short meshID, short weaponID);
    void baddy_sword7(short boneID, short meshID, short weaponID);
    void baddy_sword8(short boneID, short meshID, short weaponID);

private:
    ITEM_INFO* item;
    OBJECT_INFO* obj, *obj_sec;
    OBJ_ROTATION* rot;
    int* bone;
    int clip, frac, rate;
    short* frame[2];
    short* rotation1, *rotation2;
    short** mesh;
};

extern Baddy1Shape baddy1shape;