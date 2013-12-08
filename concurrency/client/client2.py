from twisted.internet import reactor, protocol
import time
import sys
import getopt

TESTCOUNT = 1000

class EchoClient(protocol.Protocol):
    global TESTCOUNT
    def __init__(self, factory):
        self.factory = factory
        self.time_elapse = []

    def connectionMade(self):
        print "connectionMade"
        self.transport.write(str(int(time.time() * 1000000)))
#self.transport.write(str(123456))

    def dataReceived(self, data):
        print "Server said:", data
        time_now = int(time.time() * 1000000)
        self.transport.write(str(time_now))
        time_send = int(data)
        time_elapse = time_now - time_send
        self.time_elapse.append(time_elapse)
        print "time elapse:", (time_now - time_send)
        if len(self.time_elapse) == TESTCOUNT:
            total = 0
            for num in self.time_elapse:
                total += num
            print "average service time:", total / TESTCOUNT
            reactor.stop()
#self.transport.loseConnection()

class EchoFactory(protocol.ClientFactory):
    def buildProtocol(self, addr):
        return EchoClient(self)

    def clientConnectionFailed(self, connector, reason):
        print "Connection failed."
        print reason
#reactor.stop()

#def clientConnectionLost(self, connector, reason):
#       print "Connection lost."
#reactor.stop()

def usage():
    print sys.argv[0], "[-H <host> | localhost] [-p <port> | 8000] [-n <test count> | 20]"
    sys.exit(2)

def main():
    try:
        (opts, args) = getopt.getopt(sys.argv[1:], "H:p:n:h")
    except getopt.GetoptError, err:
        print str(err)
        usage()
        sys.exit(2)
    
    port = 8000
    serverip = "localhost"
    global TESTCOUNT
    for (o, a) in opts:
        if o == "-H":
            serverip = a
        elif o == "-p":
            port = int(a)
        elif o == "-n":
            TESTCOUNT = int(a)
        elif o == "-h":
            usage()
    echoFactory = EchoFactory()
    reactor.connectTCP(serverip, port, echoFactory)
    reactor.run()

if __name__ == "__main__":
    main()
