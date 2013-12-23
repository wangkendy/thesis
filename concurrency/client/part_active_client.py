from twisted.internet import reactor, protocol
import time
import sys
import getopt

CLIENTCOUNT = 1000

class EchoClient(protocol.Protocol):
    global CLIENTCOUNT
    def __init__(self, factory, client_num):
        self.factory = factory
        self.client_num = client_num

    def connectionMade(self):
#print "connectionMade"
        self.factory.connectedProtocols.append(self)
        if len(self.factory.connectedProtocols) == CLIENTCOUNT:
            for protocol in self.factory.connectedProtocols:
                if protocol.client_num % 3 == 0:
                    protocol.transport.write("client No."+str(protocol.client_num))
#self.transport.write(str(123456))

    def dataReceived(self, data):
        print "Server said:", data
        msg = "client No." + str(self.client_num)
        self.transport.write(msg)
#time_send = int(data)
#time_elapse = time_now - time_send
#self.factory.time_elapse.append(time_elapse)
#print "time elapse:", (time_now - time_send)
#if len(self.factory.time_elapse) == CLIENTCOUNT:
#total = 0
#for num in self.factory.time_elapse:
#total += num
#print "average service time:", total / CLIENTCOUNT
#reactor.stop()
#self.transport.loseConnection()

class EchoFactory(protocol.ClientFactory):
    def __init__(self):
        self.connectedProtocols = []
        self.time_elapse = []
        self.client_num = 0	

    def buildProtocol(self, addr):
        client_num = self.client_num
        self.client_num += 1
        return EchoClient(self, client_num)

    def clientConnectionFailed(self, connector, reason):
        print "Connection failed."
        print reason
#reactor.stop()

#def clientConnectionLost(self, connector, reason):
#       print "Connection lost."
#reactor.stop()

def usage():
    print sys.argv[0], "[-H <host> | localhost] [-p <port> | 8000] [-n <client count> | 1000]"
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
    global CLIENTCOUNT
    for (o, a) in opts:
        if o == "-H":
            serverip = a
        elif o == "-p":
            port = int(a)
        elif o == "-n":
            CLIENTCOUNT = int(a)
        elif o == "-h":
            usage()
    i = 0
    echoFactory = EchoFactory()
    while i < CLIENTCOUNT:
        reactor.connectTCP(serverip, port, echoFactory)
        i += 1
    reactor.run()

if __name__ == "__main__":
    main()
