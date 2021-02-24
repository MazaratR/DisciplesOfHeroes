#ifndef MAPFILEFORMAT_H
#define MAPFILEFORMAT_H

#include <cstring>
#include <cstdint>
#include <memory>

namespace MapFileFormat
{
    struct FileHeader
    {
        FileHeader(){
            memset(this, 0, sizeof(*this));
        }

        uint16_t mapSize_x = 0;
        uint16_t mapSize_y = 0;
        uint8_t hasHumans   : 1;
        uint8_t hasElves    : 1;
        uint8_t hasDwarves  : 1;
        uint8_t hasUndeads  : 1;
        uint8_t             : 4;

        uint8_t getPlayersCount() const {
            return hasHumans + hasElves + hasDwarves + hasUndeads;
        }
    };

    struct FileElement
    {
        #define NAME_SIZE 32

        FileElement(){
            memset(this, 0, sizeof(*this));
        }

        bool isEmptyTile() const {
            return interactObjType == 0;
        }
        size_t getSizeofEmptyTile() const {
            return sizeof(terrainType) + sizeof(interactObjType);
        }
        size_t getSizeOfTileWithObj() const {
            return sizeof(*this);
        }
        void setInteractObjName(const std::string& _name){
            strncpy(interactObjtName, _name.c_str(), NAME_SIZE - 1);
            interactObjtName[NAME_SIZE - 1] = '\0';
        }
        std::string getInteractObjName() const{
            return std::string(interactObjtName);
        }

        uint8_t terrainType = 0;
        uint8_t interactObjType = 0;
        uint16_t objectAmount = 0;

    private:
        char interactObjtName[NAME_SIZE];

    };

} // namespace

#endif // MAPFILEFORMAT_H
