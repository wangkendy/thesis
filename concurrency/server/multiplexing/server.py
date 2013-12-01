from twisted.internet import protocol
import getopt
import sys

class Echo(protocol.Protocol):
    def dataReceived(self, data):
        self.transport.write(data)

class EchoFactory(protocol.Factory):
    def buildProtocol(self, addr):
        return Echo()

def usage():
    print sys.argv[0], "[-r <reactor type>[select | poll | epoll] | poll]"
    sys.exit(2)

def main():
    try:
        (opts, args) = getopt.getopt(sys.argv[1:], "r:h")
    except getopt.GetoptError, err:
        print str(err)
        usage()

    reactor_type = "poll"
    for (o, a) in opts:
        if o == "-r":
            reactor_type = a
        elif o == "-h":
            usage()
    reactor_list = ["select", "poll", "epoll"]
    if reactor_type not in reactor_list:
        print reactor_type, "not supported"
        sys.exit(2);
    if reactor_type == "select":
        from twisted.internet import selectreactor
        selectreactor.install()
    elif reactor_type == "poll":
        from twisted.internet import pollreactor
        pollreactor.install()
    else:
        from twisted.internet import epollreactor
        epollreactor.install()
    from twisted.internet import reactor
    print reactor.__class__
    reactor.listenTCP(8000, EchoFactory())
    reactor.run()

if __name__ == "__main__":
    main()
