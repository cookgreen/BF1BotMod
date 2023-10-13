#pragma once
#include <cstdint>
#include <d3dx9math.h>
#include <Windows.h>

#define OFFSET_ObfuscationMgr 0x14351D058
#define OFFSET_CLIENTGAMECONTEXT 0x1437F7758
#define OFFSET_GAMERENDERER 0x1439e6d08
#define ResolveRelativePtr(Address) ((ULONG_PTR)(Address) + *(__int32*)(Address) + sizeof(__int32))
#define ValidPointer( pointer ) ( pointer != NULL && (DWORD_PTR)pointer >= 0x10000 && (DWORD_PTR)pointer < 0x00007FFFFFFEFFFF)
void* DecryptPointer(DWORD64 EncryptedPtr, DWORD64 PointerKey);


namespace fb
{
    template <class T, INT Count, INT PadSize>
    class fixed_vector
    {
    public:
        T* m_firstElement;
        T* m_lastElement;
        T* m_arrayBound;
        LPVOID m_pad[PadSize];
        T m_data[Count];

    public:
        fixed_vector() {
            m_firstElement = (T*)m_data;
            m_lastElement = (T*)m_data;
            m_arrayBound = (T*)&m_data[Count];
        }

        void push_back(T* const value) {
            if (m_lastElement > m_arrayBound) {

                return;
            }
            *m_lastElement = *value;
            m_lastElement = m_lastElement + 1;
        };

        void clear() {
            m_firstElement = m_data;
            m_lastElement = m_data;
            m_arrayBound = (T*)&m_data[Count];
        }

        UINT Size() {
            return ((DWORD_PTR)m_lastElement - (DWORD_PTR)m_firstElement) / sizeof(T);
        }

        T At(INT nIndex) {
            return m_firstElement[nIndex];
        }

        T operator [](INT index) { return At(index); }
    };

    template <class T>
    class vector
    {
    private:
        T* m_firstElement;
        T* m_lastElement;
        T* m_arrayBound;
        void* vftable;
    public:
        size_t size()
        {
            return (((intptr_t)this->m_lastElement - (intptr_t)this->m_firstElement) / sizeof(T));
        }
        T at(int nIndex)
        {
            return *(T*)((intptr_t)m_firstElement + (nIndex * sizeof(T)));
        }
        T operator [](int index)
        {
            return at(index);
        }
    };
    template <typename T>
    class Array
    {
    private:
        T* m_firstElement;

    public:
        T At(INT nIndex)
        {
            if (!ValidPointer(m_firstElement))
                return (T)NULL;

            return *(T*)((DWORD64)m_firstElement + (nIndex * sizeof(T)));
        };

        T operator [](INT index) { return m_firstElement[index]; }

        int GetSize()
        {
            if (m_firstElement)
            {
                return *(int*)((DWORD_PTR)m_firstElement - sizeof(__int32));
            }
            return 0;
        }
    };

    template <typename T>
    class Tuple2
    {
    public:
        T Element1;
        T Element2;

    public:
        Tuple2(T _Element1, T _Element2)
        {
            Element1 = _Element1;
            Element2 = _Element2;
        }
    };

    class Color32
    {
    public:

        Color32(DWORD dwColor)
        {
            this->a = (dwColor >> 24) & 0xff;
            this->r = (dwColor >> 16) & 0xff;
            this->g = (dwColor >> 8) & 0xff;
            this->b = (dwColor) & 0xff;
        }

        Color32(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) :
            r(red), g(green), b(blue), a(alpha)
        {
        }

        static Color32 Yellow() { return Color32(255, 255, 000, 255); }
        static Color32 White() { return Color32(255, 255, 255, 255); }
        static Color32 Black() { return Color32(0, 0, 0, 255); }
        static Color32 Red() { return Color32(255, 0, 0, 255); }
        static Color32 Green() { return Color32(0, 255, 0, 255); }
        static Color32 Blue() { return Color32(0, 0, 255, 255); }
    private:
        union
        {
            struct
            {
                unsigned char r;
                unsigned char g;
                unsigned char b;
                unsigned char a;
            };

            unsigned long color;
        };
    };
    template< class T >
    struct WeakToken
    {
        T* m_realptr; //its value is: realptr + 8
        DWORD32 m_refcount;
    };
    template< class T >
    class WeakPtr
    {
    private:
        WeakToken<T>* m_ptr;
    public:
        T* GetData()
        {
            if (!ValidPointer(m_ptr))
                return NULL;

            if (!ValidPointer(&m_ptr->m_realptr))
                return NULL;

            T* ptr = m_ptr->m_realptr;
            if (!ValidPointer(ptr))
                return NULL;

            return (T*)((DWORD_PTR)(ptr)-0x8);
        }
    };

    template< class T >
    class EncryptedPtr
    {
    public:
        DWORD64 m_encryptedPtr;
        DWORD64 m_pointerKey;

    public:
        T* GetPtr()
        {
            return (T*)(DecryptPointer(this->m_encryptedPtr, (DWORD64)(this->m_pointerKey)));
        }
    };

    typedef D3DXMATRIXA16 LinearTransform;
    class AxisAlignedBox
    {
    public:
        D3DXVECTOR4 Min;
        D3DXVECTOR4 Max;
    };

    struct LinearTransform_AABB
    {
    public:
        LinearTransform m_Transform;
        AxisAlignedBox m_Box;
        char _pad[0x200];
    };

    class ClientPlayerManager
    {
    public:

    };
    class ClientGameContext
    {
    public:
        char pad_0x0000[0x68]; //0x0000
        ClientPlayerManager* m_clientPlayerManager; //0x0068 
        static ClientGameContext* GetInstance()
        {
            return *(ClientGameContext**)(OFFSET_CLIENTGAMECONTEXT);
        }
    };
    class HealthComponent
    {
    public:
        char pad_0000[32]; //0x0000
        float m_Health; //0x0020
        float m_MaxHealth; //0x0024
        char pad_0028[24]; //0x0028
        float m_VehicleHealth; //0x0040
        char pad_0044[4092]; //0x0044
    }; //Size: 0x1040
    class VehicleEntityData
    {
    public:
        char pad_0000[504]; //0x0000
        float m_FrontMaxHealth; //0x01F8
        char pad_01FC[252]; //0x01FC
        char* m_namesid; //0x02F8
        char pad_0300[7488]; //0x0300
    }; //Size: 0x2040

    class ClientVehicleEntity
    {
    public:
        virtual void Function0(); //
        virtual void Function1(); //
        virtual void Function2(); //
        virtual void Function3(); //
        virtual void Function4(); //
        virtual void Function5(); //
        virtual void Function6(); //
        virtual void Function7(); //
        virtual void Function8(); //
        virtual void Function9(); //
        virtual void Function10(); //
        virtual void Function11(); //
        virtual void Function12(); //
        virtual void Function13(); //
        virtual void Function14(); //
        virtual void Function15(); //
        virtual void Function16(); //
        virtual void Function17(); //
        virtual void Function18(); //
        virtual void Function19(); //
        virtual void Function20(); //
        virtual void Function21(); //
        virtual void Function22(); //
        virtual void Function23(); // 
        virtual void Function24();
        virtual void GetTransformAABB(LinearTransform_AABB& mTransform);// 26
        HealthComponent* GetHealthComponent()
        {
            return *(HealthComponent**)((BYTE*)this + 0x1D0);
        };

        VehicleEntityData* GetEntityData()
        {
            return *(VehicleEntityData**)((BYTE*)this + 0x30);
        };
    }; //Size: 0x0048



    class UpdatePoseResultData
    {
    public:
        class QuatTransform
        {
        public:
            D3DXVECTOR4 m_TransAndScale; //0x0000 
            D3DXVECTOR4 m_Rotation; //0x0010 
        };//Size=0x0020
        char pad_0000[24]; //0x0000
        UpdatePoseResultData::QuatTransform* m_ActiveWorldTransforms; //0x0020 
        UpdatePoseResultData::QuatTransform* m_ActiveLocalTransforms; //0x0028
        int m_Slot; //0x0030 
        int m_ReaderIndex; //0x0034 
        bool m_ValidTransforms; //0x0038 
        bool m_PoseUpdateEnabled; //0x0039 
        bool m_PoseNeeded; //0x003A 
    }; //Size: 0x1040

    class BoneCollisionComponent
    {
    public:
        UpdatePoseResultData m_ragdollTransforms; //0x0000
    }; //Size: 0x0008

    class ClientSoldierWeaponsComponent
    {
    public:
        enum WeaponSlot
        {
            M_PRIMARY = 0,
            M_SECONDARY = 1,
            M_GADGET = 2,
            M_GRENADE = 6,
            M_KNIFE = 7
        };

        char pad_0x0000[0x8A8]; //0x0000
        DWORD64 m_handler; //0x08A8 
        char pad_0x08B0[0x160]; //0x08B0
        DWORD32 m_activeSlot; //0x0A10 
        char pad_0x0A14[0x62C]; //0x0A14
    };
    class BulletEntityData
    {
    public:
        char pad_0x0000[0x140]; //0x0000
        float m_Gravity; //0x0140 
        char pad_0x0144[0x6FC]; //0x0144
    };
    class ShotConfigData
    {
    public:
        char pad_0000[168]; //0x0000
        float m_initialSpeed; //0x00A8
        char pad_00AC[52]; //0x00AC
        BulletEntityData* m_pProjectileData; //0x00E0
        char pad_00E8[28]; //0x00E8
        uint8_t shellbullet; //0x0104
        char pad_0105[287]; //0x0105
        float speedfire; //0x0224
        float speedfire2; //0x0228
        char pad_022C[3609]; //0x022C

    }; //Size=0x0808
    class PrimaryFire
    {
    public:
        char pad_0x0000[0x10]; //0x0000
        ShotConfigData* m_shotConfigData; //0x0010 
        char pad_0x0018[0x7F0]; //0x0018

    }; //Size=0x0808
    class GunSwayData
    {
    public:

    };
    class WeaponSway
    {
    public:
        char pad_0000[8]; //0x0000
        GunSwayData* m_pSwayData; //0x0008
        char pad_0010[48]; //0x0010
    };
    class WeaponFiring
    {
    public:
        char pad_0000[80]; //0x0000
        WeaponSway* m_pSway; //0x0050
        char pad_0058[216]; //0x0058
        PrimaryFire* m_pPrimaryFire; //0x0130
        char pad_0138[3848]; //0x0138

    }; //Size=0x0808
    class ClientSoldierWeapon
    {
    public:
        char pad_0x0000[0x4A48]; //0x0000
        WeaponFiring* m_pPrimary; //0x4A48 
        char pad_0x4A50[0x5F0]; //0x4A50

    };

    class ClientSoldierEntity
    {
    public:
        char pad_0000[464]; //0x0000
        HealthComponent* healthcomponent; //0x01D0
        char pad_01D8[696]; //0x01D8
        BoneCollisionComponent* bonecollisioncomponent; //0x0490
        char pad_0498[363]; //0x0498
        uint8_t N00000670; //0x0603
        float authorativeYaw; //0x0604
        char pad_0608[41]; //0x0608
        uint8_t N00000521; //0x0631
        char pad_0632[6]; //0x0632
        uint8_t poseType; //0x0638
        char pad_0639[176]; //0x0639
        uint8_t N00000538; //0x06E9
        uint8_t N0000022B; //0x06EA
        uint8_t occluded; //0x06EB
        char pad_06EC[669]; //0x06EC
        uint8_t N0000058C; //0x0989
        char pad_098A[6]; //0x098A
        D3DXVECTOR3 location; //0x0990
        char pad_099C[1712]; //0x099C
        ClientSoldierWeaponsComponent* GETsoldierWeaponsComponent()
        {
            return *(ClientSoldierWeaponsComponent**)((BYTE*)this + 0x698);
        };
        bool IsValid()
        {
            return (this->healthcomponent->m_Health > 0.1f && this->healthcomponent->m_Health <= this->healthcomponent->m_MaxHealth);
        }
        bool IsDead()
        {
            return !(this->healthcomponent->m_Health > 0.1f);
        }
        bool IsVisible()
        {
            return (this->occluded == false);
        }
        AxisAlignedBox GetAABB()
        {
            AxisAlignedBox aabb = AxisAlignedBox();
            if (this->poseType == 0) // standing
            {
                aabb.Min = D3DXVECTOR4(-0.350000f, 0.000000f, -0.350000f, 0);
                aabb.Max = D3DXVECTOR4(0.350000f, 1.700000f, 0.350000f, 0);
            }
            if (this->poseType == 1) // crouching
            {
                aabb.Min = D3DXVECTOR4(-0.350000f, 0.000000f, -0.350000f, 0);
                aabb.Max = D3DXVECTOR4(0.350000f, 1.150000f, 0.350000f, 0);
            }
            if (this->poseType == 2) // prone
            {
                aabb.Min = D3DXVECTOR4(-0.350000f, 0.000000f, -0.350000f, 0);
                aabb.Max = D3DXVECTOR4(0.350000f, 0.400000f, 0.350000f, 0);
            }
            return aabb;
        }
        bool GetBonePos(int BoneId, D3DXVECTOR3* vOut);


    }; //Size: 0x104C

    class ClientPlayer
    {
    public:
        virtual ~ClientPlayer();
        virtual DWORD_PTR GetCharacterEntity(); //=> ClientSoldierEntity + 0x268 
        virtual DWORD_PTR GetCharacterUserData(); //=> PlayerCharacterUserData
        virtual class EntryComponent* GetEntryComponent();
        virtual bool InVehicle();
        virtual unsigned int getId();
        char _0x0008[16];
        char* name; //0x0018
        char pad_0020[32]; //0x0020
        char szName[8]; //0x0040
        char pad_0048[7144]; //0x0048
        uint8_t N00000393; //0x1C30
        uint8_t N0000042C; //0x1C31
        char pad_1C32[2]; //0x1C32
        uint8_t teamId; //0x1C34
        char pad_1C35[259]; //0x1C35
        ClientVehicleEntity* clientVehicleEntity; //0x1D38
        char pad_1D40[8]; //0x1D40
        ClientSoldierEntity* clientSoldierEntity; //0x1D48
        char pad_1D50[736]; //0x1D50
    };

    class RenderView
    {
    public:
        char pad_0000[1120]; //0x0000
        D3DXMATRIXA16 viewProj; //0x0460
        char pad_04A0[3040]; //0x04A0

    }; //Size: 0x05C0

    class GameRenderer
    {
    public:
        char pad_0000[96]; //0x0000
        class RenderView* renderView; //0x0060
        char pad_0068[4112]; //0x0068

        static GameRenderer* GetInstance()
        {
            return *(GameRenderer**)OFFSET_GAMERENDERER;
        }
    }; //Size: 0x0088

    enum BoneIds
    {
        BONE_Head = 0x35,
        BONE_Neck = 0x33,
        BONE_SPINE2 = 0x7,
        BONE_SPINE1 = 0x6,
        BONE_Spine = 0x5,
        BONE_LeftShoulder = 0x8,
        BONE_RightShoulder = 0xA3,
        BONE_LeftElbowRoll = 0xE,
        BONE_RightElbowRoll = 0xA9,
        BONE_LeftHand = 0x10,
        BONE_RightHand = 0xAB,
        BONE_RightKneeRoll = 0x12D,
        BONE_LeftKneeRoll = 0x11F,
        BONE_LeftFoot = 0x115,
        BONE_RightFoot = 0x123
    };
}