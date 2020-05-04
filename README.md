# cop
Communication protocol. A comunication protocol for embedded devices written in C++. This is a header only library developed in TDD style.

The basic idea is to have a serial communication protocol where one can define Commands (they are for sure delired like e.g. TCP) and one can send events (no garanties to be delivered e.g. like UDP).

To send an Event just inherit from Event.
To be able to parse your data one needs to declare a templated parse function.


Like:

struct myEvent : cop::Event<myId> {
int iData;
double dData;

template<class Coder>
auto parse(Coder coder) {
  return coder | iData | dData
}
};

cop::Channel channel;
channel.send(myEvent());
