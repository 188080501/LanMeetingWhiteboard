#include "JdeCompute.h"

using namespace Jde;

void Jde::encryption(const void *source, const int &size, void *target, Key *key)
{
    if(size)
    {
        uint8_t const *from = (uint8_t const *)source;
        uint8_t *to = (uint8_t *)target;
        int n = (size + 7) / 8;

        if(key)
        {
            switch(size % 8)
            {
                case 0:	do { *to++ = (*from++) + key->nextValue();
                case 7:      *to++ = (*from++) + key->nextValue();
                case 6:      *to++ = (*from++) + key->nextValue();
                case 5:      *to++ = (*from++) + key->nextValue();
                case 4:      *to++ = (*from++) + key->nextValue();
                case 3:      *to++ = (*from++) + key->nextValue();
                case 2:      *to++ = (*from++) + key->nextValue();
                case 1:      *to++ = (*from++) + key->nextValue();
                    } while (--n > 0);
            }
        }
        else
        {
            switch(size % 8)
            {
                case 0:	do { *to++ = (*from++);
                case 7:      *to++ = (*from++);
                case 6:      *to++ = (*from++);
                case 5:      *to++ = (*from++);
                case 4:      *to++ = (*from++);
                case 3:      *to++ = (*from++);
                case 2:      *to++ = (*from++);
                case 1:      *to++ = (*from++);
                    } while (--n > 0);
            }
        }
    }
}

void Jde::decrypt(const void *source, const int &size, void *target, Key *key)
{
    if(size)
    {
        uint8_t const *from = (uint8_t const *)source;
        uint8_t *to = (uint8_t *)target;
        int n = (size + 7) / 8;

        if(key)
        {
            switch(size % 8)
            {
                case 0:	do { *to++ = (*from++) - key->nextValue();
                case 7:      *to++ = (*from++) - key->nextValue();
                case 6:      *to++ = (*from++) - key->nextValue();
                case 5:      *to++ = (*from++) - key->nextValue();
                case 4:      *to++ = (*from++) - key->nextValue();
                case 3:      *to++ = (*from++) - key->nextValue();
                case 2:      *to++ = (*from++) - key->nextValue();
                case 1:      *to++ = (*from++) - key->nextValue();
                    } while (--n > 0);
            }
        }
        else
        {
            switch(size % 8)
            {
                case 0:	do { *to++ = (*from++);
                case 7:      *to++ = (*from++);
                case 6:      *to++ = (*from++);
                case 5:      *to++ = (*from++);
                case 4:      *to++ = (*from++);
                case 3:      *to++ = (*from++);
                case 2:      *to++ = (*from++);
                case 1:      *to++ = (*from++);
                    } while (--n > 0);
            }
        }
    }
}
