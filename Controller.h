#pragma once
#include "cl_base.h"
#include "KKMParts.h"

struct Session
{
    int price = 0;
    int amount = 0;
};

enum Mode
{
    TicketSelection,
    MoneyInsertion
};

class Controller : public cl_base
{
    private:
        std::vector<Session*> Timetable;
        std::map<int, int> CashDrawer = { {5,0},{10,0},{50,0},{100,0},{200,0},{500,0},{1000,0} };
        int revenue = 0;
        Mode mode = TicketSelection;
        Session* reserved_session = nullptr;
        int reserved_amount = 0;

    public:
        Controller(cl_base* parent, std::string name);
        void InitSessions(std::string command);
        void FillSeats(std::string command);
        void FillPrice(std::string command);
        void FillChange(std::string command);
        void GetCashInfo(std::string& argument);
        void GetRevenue(std::string& argument);
        void GetUnsoldTickets(std::string& argument);
        void GetChangeSum(std::string& argument);
        void TicketReservationCommand(std::string command);
        void ReserveTicket(std::string& argument);
        bool CheckChangePosibility();
};

