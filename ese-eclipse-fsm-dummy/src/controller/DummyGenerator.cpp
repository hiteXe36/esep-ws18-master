#include "DummyGenerator.h"
#include "../servicetools/enums.h"

//#ifdef __DUMMY_TEST
//#include "../hal/DummyHAL.h"
//#else
//#include "../hal/HAL.h"
//#endif
//#include "../servicetools/Logger.h"
#include <string>
#include <iostream>

DummyGenerator::DummyGenerator(Channel& toDisp)
        : _2disp(toDisp)
{

}

void DummyGenerator::operator()()
{
    while(true)
    {
        std::string input;
        std::cin >> input;
//-------------------------------------------------------------------------------------------------------
//----------------------------------------LB-SIGNALS-----------------------------------------------------
//-------------------------------------------------------------------------------------------------------
        if(input.compare("ENTRYR") == 0) //Der Main mitteilen, dass ENTRY durchbrochen wurde.
        {
            _2disp.enqueue(Message(Signal::LB_ENTRY_ENTERED, Component::MAIN, Component::DUMMYGENERATOR));
        }
        else if(input.compare("ENTRYF") == 0) //Der Main mitteilen, dass ENTRY verlassen wurde.
        {
            _2disp.enqueue(Message(Signal::LB_ENTRY_LEFT, Component::MAIN, Component::DUMMYGENERATOR));
        }
        else if(input.compare("HEIGHTR") == 0) //Der Main mitteilen, dass HEIGHT durchbrochen wurde.
        {
            _2disp.enqueue(Message(Signal::LB_HIGHT_ENTERED, Component::MAIN, Component::DUMMYGENERATOR));
        }
        else if(input.compare("HEIGHTF") == 0) //Der Main mitteilen, dass HEIGHT verlassen wurde.
        {
            _2disp.enqueue(Message(Signal::LB_HIGHT_LEFT, Component::MAIN, Component::DUMMYGENERATOR));
        }
        else if(input.compare("GATER") == 0) //Der Main mitteilen, dass GATE durchbrochen wurde.
        {
            _2disp.enqueue(Message(Signal::LB_METAL_ENTERED, Component::MAIN, Component::DUMMYGENERATOR));
        }
        else if(input.compare("GATEF") == 0) //Der Main mitteilen, dass GATE verlassen wurde.
        {
            _2disp.enqueue(Message(Signal::LB_METAL_LEFT, Component::MAIN, Component::DUMMYGENERATOR));
        }
        else if(input.compare("EXITR") == 0) //Der Main mitteilen, dass EXIT durchbrochen wurde.
        {
            _2disp.enqueue(Message(Signal::LB_EXIT_ENTERED, Component::MAIN, Component::DUMMYGENERATOR));
        }
        else if(input.compare("EXITF") == 0) //Der Main mitteilen, dass HEIGHT verlassen wurde.
        {
            _2disp.enqueue(Message(Signal::LB_EXIT_LEFT, Component::MAIN, Component::DUMMYGENERATOR));
        }
        else if(input.compare("SLIDER") == 0) //Der Main mitteilen, dass SLIDE durchbrochen wurde.
        {
            _2disp.enqueue(Message(Signal::LB_SLIDE_ENTERED, Component::MAIN, Component::DUMMYGENERATOR));
        }
        else if(input.compare("SLIDEF") == 0) //Der Main mitteilen, dass SLIDE verlassen wurde.
        {
            _2disp.enqueue(Message(Signal::LB_SLIDE_LEFT, Component::MAIN, Component::DUMMYGENERATOR));
        }
        //-------------------------------------------------------------------------------------------------------
        //----------------------------------------COMM-SIGNALS-----------------------------------------------------
        //-------------------------------------------------------------------------------------------------------
        else if(input.compare("2BUSY") == 0) //Der Main mitteilen, dass Band 2 jetzt beschäftigt ist.
        {
            _2disp.enqueue(Message(Signal::MACHINE2BUSY, Component::MAIN, Component::DUMMYGENERATOR));
        }
        else if(input.compare("2IDLE") == 0) //Der Main mitteilen, dass Band 2 jetzt bereit ist.
        {
            _2disp.enqueue(Message(Signal::MACHINE2IDLE, Component::MAIN, Component::DUMMYGENERATOR));
        }
        else if(input.compare("PASSC") == 0) //Der Main mitteilen, dass Band 2 das Werkstück erfolgreich entgegengenommen hat.
        {
            _2disp.enqueue(Message(Signal::PASS_CONFIRMED, Component::MAIN, Component::DUMMYGENERATOR));
        }
            /*______________________________________________*/        else if(input.compare("NOTIFY") == 0) //Der anderen Maschine mitteilen, dass Band 1 ein PUK übergeben will
            /*_Eigentlich unnötig in diesem Dummy-Kontext___*/        {
            /*_Wenn du den Dummy-Serial-C testen willst,____*/            _2disp.enqueue(Message(Signal::NOTIFY_MACHINE2, Component::OTHER_MACHINE, Component::DUMMYGENERATOR));
            /*_____dann meinetwegen...______________________*/        }
        //-------------------------------------------------------------------------------------------------------
        //----------------------------------------HIGHT-SIGNALS-----------------------------------------------------
        //-------------------------------------------------------------------------------------------------------
        else if(input.compare("GETPUKDATA") == 0) //Den Height-Controller auffordern, Messdaten zur Main zu schicken.
        {
//            _2disp.enqueue(Message(Signal::GETPUKDATA, Component::HIGHT, Component::DUMMYGENERATOR));
            Message m(Signal::GETPUKDATA, Component::HIGHT, Component::DUMMYGENERATOR);
            std::cout << "specify id";
            std::string moreinput;
            std::cin >> moreinput;
            m._idinfo = std::stoi(moreinput);
            _2disp.enqueue(m);
        }
        else if(input.compare("RECEIVEDATA") == 0) //Den Height-Controller auffordern, Messdaten zu speichern (dies würde in echt fortlaufend automatisch geschehen)
        {
            Message m(Signal::RECEIVEDATA, Component::HIGHT, Component::DUMMYGENERATOR);
            std::cout << "specify type";
            std::string moreinput;
            std::cin >> moreinput;
            PUKType pt = static_cast<PUKType>(std::stoi(moreinput));
            if(pt != 1 && pt != 21 && pt != 22 && pt != 31 && pt != 32 && pt != 41 && pt != 42 && pt != 44 && pt != 45 && pt != 0 && pt != 99 && pt != 4711)
            {
                std::cerr << "Does not exist" << std::endl;
            }
            else
            {
                m._pukdata = pt;

                _2disp.enqueue(m);
            }
        }
        //-------------------------------------------------------------------------------------------------------
        //----------------------------------------TIMER-SIGNALS-----------------------------------------------------
        //-------------------------------------------------------------------------------------------------------
        else if(input.compare("TIMEOUT") == 0) //Der Main einen Timeout mitteilen
        {
            Message m(Signal::TIMEOUT, Component::MAIN, Component::DUMMYGENERATOR);
            std::cout << "specify id";
            std::string moreinput;
            std::cin >> moreinput;
            m._timerinfo._id = std::stoi(moreinput);
            std::cout << "specify logic_id";
            std::cin >> moreinput;
            m._timerinfo._logic_id = std::stoi(moreinput);
            _2disp.enqueue(m);
        }
            /*______________________________________________*/        else if(input.compare("STOPALLTIMERS") == 0) //Den Timer-Controller auffordern, alle Timer zu stoppen.
            /*_Eigentlich unnötig in diesem Dummy-Kontext___*/        {
            /*_Wenn du den Dummy-TImer-C testen willst,_____*/            _2disp.enqueue(Message(Signal::STOPALLTIMERS, Component::TIMER, Component::DUMMYGENERATOR));
            /*_____dann meinetwegen...______________________*/        }
            /*______________________________________________*/        else if(input.compare("STARTALLTIMERS") == 0) //Den Timer-Controller auffordern, alle Timer fortzusetzen
            /*_Eigentlich unnötig in diesem Dummy-Kontext___*/        {
            /*_Wenn du den Dummy-TImer-C testen willst,_____*/            _2disp.enqueue(Message(Signal::CONTINUEALLTIMERS, Component::TIMER, Component::DUMMYGENERATOR));
            /*_____dann meinetwegen...______________________*/        }

//        else if(input.compare("GETPUKDATA") == 0)
//        {
//            _2disp.enqueue(Message(Signal::GETPUKDATA, Component::HIGHT, Component::DUMMYGENERATOR));
//        }
//        else if(input.compare("GETPUKDATA") == 0)
//        {
//            _2disp.enqueue(Message(Signal::GETPUKDATA, Component::HIGHT, Component::DUMMYGENERATOR));
//        }
//        else if(input.compare("GETPUKDATA") == 0)
//        {
//            _2disp.enqueue(Message(Signal::GETPUKDATA, Component::HIGHT, Component::DUMMYGENERATOR));
//        }
//        else if(input.compare("GETPUKDATA") == 0)
//        {
//            _2disp.enqueue(Message(Signal::GETPUKDATA, Component::HIGHT, Component::DUMMYGENERATOR));
//        }

    }
}

DummyGenerator::~DummyGenerator()
{

}

