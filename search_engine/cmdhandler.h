#ifndef CMDHANDLER_H
#define CMDHANDLER_H
#include "handler.h"
#include "searcheng.h"


class QuitHandler : public Handler
{
public:
    QuitHandler();
    QuitHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};


class PrintHandler : public Handler
{
public:
    PrintHandler();
    PrintHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

class IncomingHandler : public Handler
{
public:
    IncomingHandler();
    IncomingHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};


class OutgoingHandler : public Handler
{
public:
    OutgoingHandler();
    OutgoingHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};


class ANDHandler : public Handler
{
public:
    ANDHandler();
    ANDHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);

};


class ORHandler : public Handler
{
public:
    ORHandler();
    ORHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);


};


class DIFFHandler : public Handler
{
public:
    DIFFHandler();
    DIFFHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);


};

class PRANDHandler : public Handler
{
public:
    PRANDHandler();
    PRANDHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);


};


class PRORHandler : public Handler
{
public:
    PRORHandler();
    PRORHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);


};


#endif
