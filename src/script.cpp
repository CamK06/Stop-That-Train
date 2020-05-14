/*
                                      THIS FILE IS A MODDED VERSION OF A FILE PART OF GTA V SCRIPT HOOK SDK
                                                                http://dev-c.com
                                                            (C) Alexander Blade 2015
*/

#include "script.h"

int timer = 0;
int count = 0;
int mytable[55 * 2 + 2];

Ped playerPed;
Vehicle myEngine;

void update_features()
{
    if (timer == 0) {
        // Get nearby vehicles then iterate over each one
        updateTable();
        for (int i = 0; i < count; i++) {
            Vehicle veh = mytable[i * 2 + 2]; // Currently iterated vehicle

            if (veh != NULL && ENTITY::DOES_ENTITY_EXIST(veh)) {
                // Set the current train engine (myEngine) if the vehicle is a train
                if (VEHICLE::IS_THIS_MODEL_A_TRAIN(veh)) {
                    myEngine = veh;
                }

                // If the vehicle is indeed a train, set all of the properties to allow it to "go boom"
                if (VEHICLE::IS_THIS_MODEL_A_TRAIN(ENTITY::GET_ENTITY_MODEL(veh))) {
                    VEHICLE::SET_VEHICLE_EXPLODES_ON_HIGH_EXPLOSION_DAMAGE(veh, true);
                    VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, true);
                    VEHICLE::SET_VEHICLE_CAN_BREAK(veh, true);
                    VEHICLE::SET_VEHICLE_STRONG(veh, false);

                    ENTITY::SET_ENTITY_HEALTH(veh, .5); // Set the train's health to half of its original value, this is so it takes one explosion rather than two to stop
                    ENTITY::SET_ENTITY_INVINCIBLE(veh, false);
                    ENTITY::SET_ENTITY_PROOFS(veh, true, true, false, false, true, false, false, false);
                    ENTITY::SET_ENTITY_HAS_GRAVITY(veh, true);

                    for (int i = 1; i < 100; i++) // Iterate over 100 train cars (the max GTA allows), this is for compatibility with train mods
                    {
                        Vehicle car = VEHICLE::GET_TRAIN_CARRIAGE(myEngine, i);
                        if (car != NULL && ENTITY::DOES_ENTITY_EXIST(car))
                            if (ENTITY::IS_ENTITY_DEAD(myEngine))
                            {
                                VEHICLE::DELETE_VEHICLE((Vehicle*)car);
                            }
                            else break;
                    }
                }

                if (ENTITY::IS_ENTITY_DEAD(myEngine) || checkPrevious(myEngine)) {
                    VEHICLE::SET_RENDER_TRAIN_AS_DERAILED(myEngine, true);
                }

                timer = 30;
            }
        }
    }
    else if (timer == 15) {
        // Null every vehicle value
        for (int i = 0; i < count; i++) {
            mytable[i * 2 + 2] = NULL;
        }

        timer--;
    }
    else timer--;


    // Slow down the train if it has exploded
    if (ENTITY::IS_ENTITY_DEAD(myEngine) || checkPrevious(myEngine)) {
        VEHICLE::SET_RENDER_TRAIN_AS_DERAILED(myEngine, true);

        if (ENTITY::GET_ENTITY_SPEED(myEngine) > 1.5)
            VEHICLE::SET_TRAIN_SPEED(myEngine, (ENTITY::GET_ENTITY_SPEED(myEngine) * .9));
        else if (ENTITY::GET_ENTITY_SPEED(myEngine) > 0.1)
            VEHICLE::SET_TRAIN_SPEED(myEngine, (ENTITY::GET_ENTITY_SPEED(myEngine) - .01) - (ENTITY::GET_ENTITY_SPEED(myEngine) * 1.1));
        else if (ENTITY::GET_ENTITY_SPEED(myEngine) < 0)
            VEHICLE::SET_TRAIN_SPEED(myEngine, 0);
    }
}

// TO BE FIXED/IMPLEMENTED IN VERSION 1.1

bool checkPrevious(Vehicle traincar)
{
    bool isDestroyed = false;

    for (int i = 1; i < 100; i++) // Iterate over 100 train cars (the max GTA allows), this is for compatibility with train mods
    {
        Vehicle car = VEHICLE::GET_TRAIN_CARRIAGE(traincar, i);
        if (car != NULL && ENTITY::DOES_ENTITY_EXIST(car))
            if (ENTITY::IS_ENTITY_DEAD(traincar))
            {
                isDestroyed = true;
                VEHICLE::EXPLODE_VEHICLE(car, TRUE, FALSE);
            }
        else break;
    }

    return isDestroyed;
}


void updateTable()
{
    playerPed = PLAYER::PLAYER_PED_ID();

    // Get nearby vehicles
    mytable[0] = 55;
    count = PED::GET_PED_NEARBY_VEHICLES(playerPed, mytable);
}

/*
void Log(const char* text)
{
    std::ofstream out("VulnerableTrains.log", std::ios::app);
    out << text << "\n";
}
*/

void main()
{
    while (true)
    {
        update_features();
        WAIT(0); // Lets other scripts run before returning to this one.
    }
}

void ScriptMain()
{
    srand(GetTickCount());
    main();
}