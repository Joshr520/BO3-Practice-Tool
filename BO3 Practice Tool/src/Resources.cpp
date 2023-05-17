#pragma warning( disable : 4244 ) 

#include "Resources.h"
#include "GUIFunctions.h"
#include <limits.h>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <locale>
#include "json.h"

#include "Walnut/Random.h"

namespace ZombieCalc
{
    int GetZombieCountForRound(int round, int playerCount)
    {
        int maxZombies = 24;
        float multiplier = round / 5.f;
        if (multiplier < 1)
            multiplier = 1;
        if (round >= 10)
            multiplier *= (round * 0.15);
        if (playerCount == 1)
            maxZombies += 3 * multiplier;
        else
            maxZombies += ((playerCount - 1) * 6) * multiplier;
        return MaxZombieFunc(maxZombies, round);
    }

    int MaxZombieFunc(int maxZombies, int round)
    {
        if (round < 2)
            return maxZombies * 0.25;
        if (round < 3)
            return maxZombies * 0.3;
        if (round < 4)
            return maxZombies * 0.5;
        if (round < 5)
            return maxZombies * 0.7;
        if (round < 6)
            return maxZombies * 0.9;
        return maxZombies;
    }

    int GetZombiesUpToRound(int round, int playerCount)
    {
        if (round == 1)
            return 0;
        return GetZombieCountForRound(round - 1, playerCount) + GetZombiesUpToRound(round - 1, playerCount);
    }

    int GetZombieHealthForRound(int round)
    {
        if (round > 162) return INT_MAX;
        int zombieHealth = 150;
        if(round < 10) zombieHealth += 100 * (round - 1);
        else zombieHealth += 100 * (9 - 1);
        if (round < 10)
            return zombieHealth;
        for (int i = 10; i <= round; i++)
        {
            zombieHealth += (int)zombieHealth * 0.1;
        }
        return zombieHealth;
    }

    int GetZombieSpawnRateForRound(int round, int playerCount)
    {
        if (round == 1)
            return 2100;
        float startDelay = 2.0f;
        if (playerCount == 2)
            startDelay = 1.5f;
        else if (playerCount == 3)
            startDelay = 0.89f;
        else if (playerCount == 4)
            startDelay = 0.67f;
        float calcSpawnRate = startDelay * pow(0.95f, round - 1) + 0.1f;
        if (calcSpawnRate < 0.2f)
            calcSpawnRate = 0.2f;
        int calcRoundedTime = calcSpawnRate * 1000;
        return RoundTimeNearest50(calcRoundedTime);
    } 

    float RawSpawnRateForRound(int round, int playerCount)
    {
        if(round == 1)
            return 2.1f;
        float startDelay = 2.0f;
        if (playerCount == 2)
            startDelay = 1.5f;
        else if (playerCount == 3)
            startDelay = 0.89f;
        else if (playerCount == 4)
            startDelay = 0.67f;
        return startDelay * pow(0.95f, round - 1);
    }

    int RoundTimeNearest50(int time)
    {
        int newTime = time % 50;
        if (newTime < 25)
            newTime = time - newTime;
        else
            newTime = time + (50 - newTime);
        return newTime;
    }

    int GetSpecialEnemySpawnRate(int playerCount, const std::string& specialEnemy)
    {
        switch (hashstr(specialEnemy.c_str()))
        {
        case hashstr("Meatballs"):
        case hashstr("Manglers"):
        case hashstr("Valks"):
            switch (playerCount)
            {
            case 1:
                return 2350;
            case 2:
                return 1850;
            case 3:
                return 1350;
            case 4:
                return 850;
            }
            break;
        case hashstr("Dogs"):
            switch (numDogRounds)
            {
            case 1:
                return 2100;
            case 2:
                return 1600;
            case 3:
                return 1100;
            default:
                return 600;
            }
            break;
        case hashstr("Spiders"):
            switch (playerCount)
            {
            case 1:
                return 2350;
            case 2:
                return 2100;
            case 3:
                return 1850;
            case 4:
                return 1600;
            }
            break;
        case hashstr("Furys"):
            return 2750;
        case hashstr("Keepers"):
            return 1400;
        }
        return 0;
    }

    std::string CalcRoundTime(int round, int playerCount, int corpseDelay, bool specialEnemies)
    {
        int roundIntermission = 12600;
        if (round == 1)
            roundIntermission = 8150;
        int zombieCount = GetZombieCountForRound(round, playerCount);
        int specialEnemiesSpawnTime = 0;
        if (specialEnemies)
        {
            zombieCount -= specialEnemyCount_1 + specialEnemyCount_2 + specialEnemyCount_3;
            if (specialEnemyCount_1)
                specialEnemiesSpawnTime += GetSpecialEnemySpawnRate(playerCount, specialEnemy_1) * specialEnemyCount_1;
            if (specialEnemyCount_2)
                specialEnemiesSpawnTime += GetSpecialEnemySpawnRate(playerCount, specialEnemy_2) * specialEnemyCount_2;
            if (specialEnemyCount_3)
                specialEnemiesSpawnTime += GetSpecialEnemySpawnRate(playerCount, specialEnemy_3) * specialEnemyCount_3;
        }
        int zombieSpawnsTime = GetZombieSpawnRateForRound(round, playerCount) * (zombieCount - 1) + specialEnemiesSpawnTime + roundIntermission;
        if (corpseDelay) zombieSpawnsTime += 100 * corpseDelay;
        return ParseTimeFromMilli(zombieSpawnsTime);
    }

    std::string SpecialRoundTime(int round, int playerCount, int corpseDelay, bool soe)
    {
        int roundIntermission = 12600;
        if (round == 1)
            roundIntermission = 8150;
        int zombieCount = GetZombieCountForRound(round, playerCount);
        int zombieSpawnsTime;
        if (soe)
            zombieSpawnsTime = 2100 * (zombieCount - 1);
        else
        {
            if (moonRoundSkip)
                zombieCount = 25;
            zombieSpawnsTime = GetZombieSpawnRateForRound(round + (moonEarthTravels * 2 - moonRoundSkips) + 1, 1) * (zombieCount - 1);
        }
        zombieSpawnsTime += +roundIntermission;
        if (corpseDelay) zombieSpawnsTime += 100 * corpseDelay;
        return ParseTimeFromMilli(zombieSpawnsTime);
    }

    std::string CustomRoundTime(int round, int playerCount, int zombieCount)
    {
        int zombieSpawnsTime = GetZombieSpawnRateForRound(round, playerCount) * (zombieCount - 1);
        return ParseTimeFromMilli(zombieSpawnsTime);
    }

    void CalcLockdownTime(int round, int playerCount)
    {
        float rawSpawnRate_1 = RawSpawnRateForRound(round, playerCount);
        if (rawSpawnRate_1 < 0.1f)
            rawSpawnRate_1 = 0.1f;
        float rawSpawnRate_2 = rawSpawnRate_1 - playerCount * 0.1;
        if (rawSpawnRate_2 < 0.1f)
            rawSpawnRate_2 = 0.1f;
        float rawSpawnRate_3 = rawSpawnRate_2 - playerCount * 0.1;
        if (rawSpawnRate_3 < 0.1f)
            rawSpawnRate_3 = 0.1f;
        float rawSpawnRate_4 = rawSpawnRate_3 - playerCount * 0.1;
        if (rawSpawnRate_4 < 0.1f)
            rawSpawnRate_4 = 0.1f;

        int spawnRate_1 = RoundTimeNearest50(rawSpawnRate_1 * 1000) + 50;
        int spawnRate_2 = RoundTimeNearest50(rawSpawnRate_2 * 1000) + 50;
        int spawnRate_3 = RoundTimeNearest50(rawSpawnRate_3 * 1000) + 50;
        int spawnRate_4 = RoundTimeNearest50(rawSpawnRate_4 * 1000) + 50;

        int waveCount_1 = 14 + (8 * playerCount);
        int waveCount_2 = 16 + (9 * playerCount);
        int waveCount_3 = 18 + (10 * playerCount);
        int waveCount_4 = 18 + (10 * playerCount * 2);

        int waveIntermission_1 = 6200;
        int waveIntermission_2 = 5500;
        int waveIntermission_3 = 5500;
        int waveIntermission_4 = 3300;

        waveTime_1 = ParseTimeFromMilli(spawnRate_1 * waveCount_1 + waveIntermission_1 + 500);
        waveTime_2 = ParseTimeFromMilli(spawnRate_2 * waveCount_2 + waveIntermission_2 + 500);
        waveTime_3 = ParseTimeFromMilli(spawnRate_3 * waveCount_3 + waveIntermission_3 + 500);
        waveTime_4 = ParseTimeFromMilli(spawnRate_4 * waveCount_4 + waveIntermission_4 + 500);
    }
}

namespace GKValveSolver
{
    void InitValveSolutions()
    {
        std::unordered_map<std::string, std::vector<std::string>> departmentSolutions;
        std::unordered_map<std::string, std::vector<std::string>> dragonSolutions;
        std::unordered_map<std::string, std::vector<std::string>> armorySolutions;
        std::unordered_map<std::string, std::vector<std::string>> supplySolutions;
        std::unordered_map<std::string, std::vector<std::string>> infirmarySolutions;
        std::unordered_map<std::string, std::vector<std::string>> tankSolutions;

        departmentSolutions.insert({ "Dragon Command", { "P", "1", "2", "2", "1", "1" } });
        departmentSolutions.insert({ "Armory", { "P", "2", "1", "3", "3", "1" } });
        departmentSolutions.insert({ "Supply", { "P", "3", "3", "1", "2", "3" } });
        departmentSolutions.insert({ "Infirmary", { "P", "2", "1", "1", "2", "3" } });
        departmentSolutions.insert({ "Tank", { "P", "1", "3", "2", "3", "1" } });

        dragonSolutions.insert({ "Department", { "1", "P", "1", "3", "3", "1" } });
        dragonSolutions.insert({ "Armory", { "3", "P", "1", "3", "1", "1" } });
        dragonSolutions.insert({ "Supply", { "3", "P", "2", "2", "1", "1" } });
        dragonSolutions.insert({ "Infirmary", { "1", "P", "2", "1", "1", "2" } });
        dragonSolutions.insert({ "Tank", { "1", "P", "1", "1", "1", "1" } });

        armorySolutions.insert({ "Department", { "2", "1", "P", "3", "3", "3" } });
        armorySolutions.insert({ "Dragon Command", { "1", "1", "P", "3", "1", "1" } });
        armorySolutions.insert({ "Supply", { "2", "2", "P", "1", "2", "3" } });
        armorySolutions.insert({ "Infirmary", { "3", "1", "P", "3", "1", "3" } });
        armorySolutions.insert({ "Tank", { "3", "1", "P", "2", "1", "1" } });

        supplySolutions.insert({ "Department", { "1", "1", "2", "P", "3", "1" } });
        supplySolutions.insert({ "Dragon Command", { "2", "2", "1", "P", "2", "3" } });
        supplySolutions.insert({ "Armory", { "3", "1", "2", "P", "1", "1" } });
        supplySolutions.insert({ "Infirmary", { "3", "1", "3", "P", "2", "3" } });
        supplySolutions.insert({ "Tank", { "1", "2", "1", "P", "3", "1" } });

        infirmarySolutions.insert({ "Department", { "1", "3", "2", "1", "P", "2" } });
        infirmarySolutions.insert({ "Dragon Command", { "2", "1", "3", "3", "P", "3" } });
        infirmarySolutions.insert({ "Armory", { "2", "2", "2", "1", "P", "2" } });
        infirmarySolutions.insert({ "Supply", { "1", "2", "2", "1", "P", "1" } });
        infirmarySolutions.insert({ "Tank", { "3", "3", "3", "2", "P", "2" } });

        tankSolutions.insert({ "Department", { "1", "3", "1", "1", "2", "P" } });
        tankSolutions.insert({ "Dragon Command", { "2", "1", "3", "2", "2", "P" } });
        tankSolutions.insert({ "Armory", { "2", "2", "1", "1", "2", "P" } });
        tankSolutions.insert({ "Supply", { "1", "3", "2", "1", "1", "P" } });
        tankSolutions.insert({ "Infirmary", { "3", "1", "2", "2", "1", "P" } });

        valveSolutions_1.insert({ "Department", departmentSolutions });
        valveSolutions_1.insert({ "Dragon Command", dragonSolutions });
        valveSolutions_1.insert({ "Armory", armorySolutions });
        valveSolutions_1.insert({ "Supply", supplySolutions });
        valveSolutions_1.insert({ "Infirmary", infirmarySolutions });
        valveSolutions_1.insert({ "Tank", tankSolutions });

        departmentSolutions = { };
        dragonSolutions = { };
        armorySolutions = { };
        supplySolutions = { };
        infirmarySolutions = { };
        tankSolutions = { };

        departmentSolutions.insert({ "Dragon Command", { "P", "3", "3", "2", "2", "2" } });
        departmentSolutions.insert({ "Armory", { "P", "3", "2", "1", "1", "2" } });
        departmentSolutions.insert({ "Supply", { "P", "2", "2", "2", "3", "1" } });
        departmentSolutions.insert({ "Infirmary", { "P", "1", "3", "3", "3", "3" } });
        departmentSolutions.insert({ "Tank", { "P", "3", "1", "1", "1", "3" } });

        dragonSolutions.insert({ "Department", { "2", "P", "1", "1", "2", "3" } });
        dragonSolutions.insert({ "Armory", { "2", "P", "3", "1", "2", "2" } });
        dragonSolutions.insert({ "Supply", { "2", "P", "3", "3", "3", "3" } });
        dragonSolutions.insert({ "Infirmary", { "3", "P", "3", "2", "2", "2" } });
        dragonSolutions.insert({ "Tank", { "2", "P", "3", "2", "3", "2" } });

        armorySolutions.insert({ "Department", { "3", "3", "P", "2", "2", "2" } });
        armorySolutions.insert({ "Dragon Command", { "2", "2", "P", "2", "2", "2" } });
        armorySolutions.insert({ "Supply", { "1", "2", "P", "3", "3", "1" } });
        armorySolutions.insert({ "Infirmary", { "1", "2", "P", "1", "2", "2" } });
        armorySolutions.insert({ "Tank", { "1", "3", "P", "1", "1", "2" } });

        supplySolutions.insert({ "Department", { "3", "3", "1", "P", "2", "3" } });
        supplySolutions.insert({ "Dragon Command", { "1", "3", "2", "P", "1", "2" } });
        supplySolutions.insert({ "Armory", { "3", "3", "3", "P", "2", "2" } });
        supplySolutions.insert({ "Infirmary", { "1", "2", "2", "P", "3", "2" } });
        supplySolutions.insert({ "Tank", { "2", "1", "3", "P", "3", "3" } });

        infirmarySolutions.insert({ "Department", { "3", "1", "2", "2", "P", "1" } });
        infirmarySolutions.insert({ "Dragon Command", { "1", "2", "1", "3", "P", "1" } });
        infirmarySolutions.insert({ "Armory", { "3", "1", "3", "3", "P", "1" } });
        infirmarySolutions.insert({ "Supply", { "3", "3", "3", "3", "P", "3" } });
        infirmarySolutions.insert({ "Tank", { "2", "2", "1", "1", "P", "3" } });

        tankSolutions.insert({ "Department", { "2", "1", "2", "2", "3", "P" } });
        tankSolutions.insert({ "Dragon Command", { "1", "3", "1", "3", "1", "P" } });
        tankSolutions.insert({ "Armory", { "2", "1", "3", "3", "3", "P" } });
        tankSolutions.insert({ "Supply", { "3", "3", "3", "2", "2", "P" } });
        tankSolutions.insert({ "Infirmary", { "1", "2", "1", "3", "3", "P" } });

        valveSolutions_2.insert({ "Department", departmentSolutions });
        valveSolutions_2.insert({ "Dragon Command", dragonSolutions });
        valveSolutions_2.insert({ "Armory", armorySolutions });
        valveSolutions_2.insert({ "Supply", supplySolutions });
        valveSolutions_2.insert({ "Infirmary", infirmarySolutions });
        valveSolutions_2.insert({ "Tank", tankSolutions });
    }

    void CalcValveProbabilities()
    {
        valveDirectionCounts_1 = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
        valveDirectionCounts_2 = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
        for (std::string& solutions : valveLocations)
        {
            if (passwordChosen && passwordLocation != solutions)
                continue;
            int checked = -1;
            for (std::pair<const std::string, std::vector<std::string>>& solution : valveSolutions_1[solutions])
            {
                checked++;
                if (std::find(excludedValves.begin(), excludedValves.end(), solution.second) != excludedValves.end())
                    continue;
                if (greenChosen && solution.first != greenLocation)
                    continue;
                int num = locationToInt[solution.first];
                if (checkedGreenArray[locationToInt[solution.first]])
                    continue;
                int index = -1;
                for (std::string& direction : solution.second)
                {
                    index++;
                    if (direction == "P")
                        continue;
                    int number = atoi(direction.c_str()) - 1;
                    valveDirectionCounts_1[index][number]++;
                }
            }
        }
        for (std::string& solutions : valveLocations)
        {
            if (passwordLocation != "" && passwordLocation != solutions)
                continue;
            int checked = -1;
            for (std::pair<const std::string, std::vector<std::string>>& solution : valveSolutions_2[solutions])
            {
                checked++;
                if (std::find(excludedValves.begin(), excludedValves.end(), solution.second) != excludedValves.end())
                    continue;
                if (greenChosen && solution.first != greenLocation)
                    continue;
                int num = locationToInt[solution.first];
                if (checkedGreenArray[locationToInt[solution.first]])
                    continue;
                int index = -1;
                for (std::string& direction : solution.second)
                {
                    index++;
                    if (direction == "P")
                        continue;
                    int number = atoi(direction.c_str()) - 1;
                    valveDirectionCounts_2[index][number]++;
                }
            }
        }
        int odds = 0;
        for(std::vector<int>& counts : valveDirectionCounts_1)
        {
            int index = -1;
            for (int& count : counts)
            {
                index++;
                int divideBy = valveDirectionCounts_1[odds][0] + valveDirectionCounts_1[odds][1] + valveDirectionCounts_1[odds][2];
                if (!divideBy)
                    valveDirectionOdds_1[odds][index] = "P";
                else
                {
                    float percentage = count * 100.0f / divideBy;
                    char buf[8];
                    sprintf_s(buf, "%.2f", percentage);
                    std::string addPercentage = buf;
                    addPercentage += "%%";
                    valveDirectionOdds_1[odds][index] = addPercentage;
                }
            }
            odds++;
        }
        odds = 0;
        for (std::vector<int>& counts : valveDirectionCounts_2)
        {
            int index = -1;
            for (int& count : counts)
            {
                index++;
                int divideBy = valveDirectionCounts_2[odds][0] + valveDirectionCounts_2[odds][1] + valveDirectionCounts_2[odds][2];
                if (!divideBy)
                    valveDirectionOdds_2[odds][index] = "P";
                else
                {
                    float percentage = count * 100.0f / divideBy;
                    char buf[8];
                    sprintf_s(buf, "%.2f", percentage);
                    std::string addPercentage = buf;
                    addPercentage += "%%";
                    valveDirectionOdds_2[odds][index] = addPercentage;
                }
            }
            odds++;
        }
        int a = 0;
    }

    void CalcRemainingGreen()
    {
        int notGreen = 0;
        std::string turnToGreen = "";
        int compareAgainst = (passwordChosen) ? 1 : 0;
        noGreenChoice = false;
        for (int i = 0; i < 6; i++)
        {
            if (checkedGreenArray[i])
            {
                if (passwordChosen && passwordLocation == valveLocations[i])
                    continue;
                notGreen++;
            }
            else
                turnToGreen = valveLocations[i];
            if (i - notGreen > compareAgainst)
                return;
        }
        if (notGreen >= 6)
            return;
        noGreenChoice = true;
        greenLocation = turnToGreen;
        valveGreen[locationToInt[greenLocation]] = true;
        greenChosen = true;
    }

    void CalcExcludedValves()
    {
        std::vector<std::vector<std::string>> results;
        std::vector<std::string> result;
        excludedValves = { };
        possibleValves_1 = { };
        possibleValves_2 = { };
        for (int valve = 0; valve < 6; valve++)
        {
            for (int direction = 0; direction < 3; direction++)
            {
                if (passwordLocation == valveLocations[valve])
                {
                    result.push_back("P");
                    break;
                }
                if (valveDirections[valve][direction])
                {
                    result.push_back(std::to_string(direction + 1));
                    break;
                }
                if (direction == 2)
                    result.push_back("0");
            }
        }
        if (result.size() != 6)
        {
            valveComboSet = false;
            return;
        }
        if (passwordChosen)
            valveComboSet = true;
        for (std::string& solutions : valveLocations)
        {
            for (std::pair<const std::string, std::vector<std::string>>& solution : valveSolutions_1[solutions])
            {
                if (result == solution.second)
                {
                    excludedValves.push_back(result);
                }
                else if (passwordLocation == solutions)
                {
                    possibleValves_1.insert({ solution.first, solution.second });
                }
            }
            for (std::pair<const std::string, std::vector<std::string>>& solution : valveSolutions_2[solutions])
            {
                if (result == solution.second)
                {
                    excludedValves.push_back(result);
                }
                else if (passwordLocation == solutions)
                {
                    possibleValves_2.insert({ solution.first, solution.second });
                }
            }
        }
    }
}

namespace IceCodePractice
{
    void InitIceCodePairs()
    {
        iceCodePairs.push_back(IceCodePair({ iceCodeImgList["dot-digit"], iceCodeImgList["dot-symbol"] }));
        iceCodePairs.push_back(IceCodePair({ iceCodeImgList["i-digit"], iceCodeImgList["i-symbol"] }));
        iceCodePairs.push_back(IceCodePair({ iceCodeImgList["l-digit"], iceCodeImgList["l-symbol"] }));
        iceCodePairs.push_back(IceCodePair({ iceCodeImgList["f-digit"], iceCodeImgList["f-symbol"] }));
        iceCodePairs.push_back(IceCodePair({ iceCodeImgList["i-dot-digit"], iceCodeImgList["i-dot-symbol"] }));
        iceCodePairs.push_back(IceCodePair({ iceCodeImgList["i-i-digit"], iceCodeImgList["i-i-symbol"] }));
        iceCodePairs.push_back(IceCodePair({ iceCodeImgList["i-l-digit"], iceCodeImgList["i-l-symbol"] }));
        iceCodePairs.push_back(IceCodePair({ iceCodeImgList["i-f-digit"], iceCodeImgList["i-f-symbol"] }));
        iceCodePairs.push_back(IceCodePair({ iceCodeImgList["l-dot-digit"], iceCodeImgList["l-dot-symbol"] }));
        iceCodePairs.push_back(IceCodePair({ iceCodeImgList["l-i-digit"], iceCodeImgList["l-i-symbol"] }));
        iceCodePairs.push_back(IceCodePair({ iceCodeImgList["l-l-digit"], iceCodeImgList["l-l-symbol"] }));
        iceCodePairs.push_back(IceCodePair({ iceCodeImgList["l-f-digit"], iceCodeImgList["l-f-symbol"] }));

        randomIceCodePairs = iceCodePairs;

        RandomizeCodes();
    }

    void RandomizeCodes()
    {
        Walnut::Random::Init();
        std::shuffle(iceCodePairs.begin(), iceCodePairs.end(), std::default_random_engine(Walnut::Random::UInt()));
        Walnut::Random::Init();
        std::shuffle(randomIceCodePairs.begin(), randomIceCodePairs.end(), std::default_random_engine(Walnut::Random::UInt()));
        Walnut::Random::Init();
        std::shuffle(&randomList[0], &randomList[11], std::default_random_engine(Walnut::Random::UInt()));
    }

    void ProgressGame(bool success, int numCode)
    {
        if (!gameStarted)
        {
            gameStarted = true;
            gameTimer.Reset();
        }
        if (success)
        {
            gameProgress++;
            for (bool& checked : gameChecked)
                checked = false;
            if (gameProgress >= 12)
            {
                gameTime = "Time: " + ParseTimeFromMilli(gameTimer.Elapsed());
                std::stringstream ss;
                ss.precision(4);
                float percentage = (timesGuessed - timesMissed) / (float)timesGuessed * 100;
                ss << percentage << "%%";
                accuracy = "Accuracy: " + ss.str();
                gameProgress = 0;
                gameStarted = false;
                timesMissed = 0;
                timesGuessed = 0;
                RandomizeCodes();
                return;
            }
            Walnut::Random::Init();
            std::shuffle(randomIceCodePairs.begin(), randomIceCodePairs.end(), std::default_random_engine(Walnut::Random::UInt()));
        }
        else if (!gameChecked[numCode])
        {
            gameChecked[numCode] = true;
            timesMissed++;
        }
        timesGuessed++;
        std::stringstream ss;
        ss.precision(4);
        float percentage = (timesGuessed - timesMissed) / (float)timesGuessed * 100;
        ss << "Accuracy: " << percentage << "%%";
        accuracy = "Accuracy: " + ss.str();
    }
}

namespace Autosplits
{
#define PRESET_DIRECTORY "/Settings/Autosplits"
    void LoadSplitPresets()
    {
        splitPresets.clear();
        for (const auto& file : std::filesystem::directory_iterator(std::filesystem::path(selfDirectory) / "Settings\\Autosplits"))
        {
            if (std::filesystem::is_regular_file(file))
            {
                SplitPreset preset = ParseSplitJson(file.path().string());
                preset.presetName = file.path().stem().string();
                splitPresets.push_back(preset);
            }
        }

        if (!splitPresets.size())
            splitPresets.push_back(inactiveSplitPreset);
    }

    void WriteAutosplitPreset(const SplitPreset& preset)
    {
        std::string filename = selfDirectory + PRESET_DIRECTORY + "/" + preset.presetName + ".json";
        Document doc;
        doc.SetObject();

        Value settings(kObjectType);
        settings.AddMember("In Game Timer", preset.igt, doc.GetAllocator());
        settings.AddMember("In Game Round Timer", preset.igrt, doc.GetAllocator());
        settings.AddMember("Amount of Splits", preset.numSplits, doc.GetAllocator());
        settings.AddMember("Map Index", preset.map, doc.GetAllocator());
        settings.AddMember("Split Type", preset.splitType, doc.GetAllocator());
        settings.AddMember("Round Interval", preset.roundInterval, doc.GetAllocator());

        Value splitData(kObjectType);
        for (std::pair<std::string, int> pair : preset.splits)
        {
            Value key(StringRef(pair.first.c_str()), doc.GetAllocator());
            Value value(pair.second);
            splitData.AddMember(key, value, doc.GetAllocator());
        }

        doc.AddMember("Settings", settings, doc.GetAllocator());
        doc.AddMember("Split Data", splitData, doc.GetAllocator());

        JSON::WriteJson(doc, filename);
        if (writeSplits && appStatus == "Status: Active")
            JSON::WriteJson(doc, bo3Directory + "\\Practice Tool\\Settings\\Active Autosplit Preset.txt");
        else
            JSON::WriteJson({ }, bo3Directory + "\\Practice Tool\\Settings\\Active Autosplit Preset.txt");
    }

    void CreateNewAutosplitPreset(const std::string& presetName)
    {
        std::string filename = selfDirectory + PRESET_DIRECTORY + "/" + presetName + ".json";
        Document doc;
        doc.SetObject();

        Value settings(kObjectType);
        settings.AddMember("In Game Timer", false, doc.GetAllocator());
        settings.AddMember("In Game Round Timer", true, doc.GetAllocator());
        settings.AddMember("Amount of Splits", 0, doc.GetAllocator());
        settings.AddMember("Map Index", 0, doc.GetAllocator());
        settings.AddMember("Split Type", 0, doc.GetAllocator());
        settings.AddMember("Round Interval", 1, doc.GetAllocator());

        Value splitData(kObjectType);

        doc.AddMember("Settings", settings, doc.GetAllocator());
        doc.AddMember("Split Data", splitData, doc.GetAllocator());

        JSON::WriteJson(doc, filename);
        LoadSplitPresets();
    }

    void DeleteAutosplitPreset(const std::string& preset)
    {
        std::string file = PRESET_DIRECTORY;
        file += "/" + preset + ".json";
        if (std::filesystem::exists(selfDirectory + file))
            std::filesystem::remove(selfDirectory + file);
        if (currentSplitPreset > 0) currentSplitPreset--;
        LoadSplitPresets();
    }

    SplitPreset ParseSplitJson(const std::string& filePath)
    {
        SplitPreset returnPreset;
        Document doc = JSON::ReadJsonFromFile(filePath);

        if (doc.HasMember("Settings") && doc["Settings"].IsObject())
        {
            const Value& settings = doc["Settings"];

            for (Value::ConstMemberIterator it = settings.MemberBegin(); it != settings.MemberEnd(); it++)
            {
                const std::string& key = it->name.GetString();
                const Value& value = it->value;

                switch (hashstr(key.c_str()))
                {
                case hashstr("In Game Timer"):
                    if (value.IsBool())
                        returnPreset.igt = value.GetBool();
                    break;
                case hashstr("In Game Round Timer"):
                    if (value.IsBool())
                        returnPreset.igrt = value.GetBool();
                    break;
                case hashstr("Amount of Splits"):
                    if (value.IsInt())
                        returnPreset.numSplits = value.GetInt();
                    break;
                case hashstr("Map Index"):
                    if (value.IsInt())
                        returnPreset.map = value.GetInt();
                    break;
                case hashstr("Split Type"):
                    if (value.IsInt())
                        returnPreset.splitType = value.GetInt();
                    break;
                case hashstr("Round Interval"):
                    if (value.IsInt())
                        returnPreset.roundInterval = value.GetInt();
                    break;
                }
            }
        }
        if (doc.HasMember("Split Data") && doc["Split Data"].IsObject())
        {
            const Value& splitData = doc["Split Data"];

            for (Value::ConstMemberIterator it = splitData.MemberBegin(); it != splitData.MemberEnd(); it++)
            {
                if (it->name.IsString() && it->value.IsInt())
                {
                    returnPreset.splits.push_back({ it->name.GetString(), it->value.GetInt() });
                    //returnPreset.numSplits++;
                }
                else
                    LogFile("Autosplits json error: Incorrect typings found at key " + std::string(it->name.GetString()));
            }
        }

        return returnPreset;
    }
}

std::string ParseTimeFromMilli(int milliseconds)
{
    std::string finalTime = "";
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    if (milliseconds >= 3600000)
    {
        hours = milliseconds / 3600000;
        if (hours < 10)
            finalTime += "0";
        finalTime += std::to_string(hours) + ":";
        milliseconds -= hours * 3600000;
    }
    if (milliseconds >= 60000)
    {
        minutes = milliseconds / 60000;
        if (minutes < 10)
            finalTime += "0";
        finalTime += std::to_string(minutes) + ":";
        milliseconds -= minutes * 60000;
    }
    else if (hours > 0)
    {
        finalTime += "00:";
    }
    if (milliseconds >= 1000)
    {
        seconds = milliseconds / 1000;
        if (seconds < 10 && (hours || minutes))
            finalTime += "0";
        finalTime += std::to_string(seconds) + ".";
        milliseconds -= seconds * 1000;
    }
    else
        if (hours || minutes)
            finalTime += "00.";
        else
            finalTime += "0.";
    if (milliseconds >= 50)
    {
        if (milliseconds < 100)
            finalTime += "0";
        finalTime += std::to_string(milliseconds);
    }
    else
        finalTime += "000";
    return finalTime;
}