import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class Client 
{

	public static void main(String[] args)
	{
		InetAddress adr;
		DatagramPacket packet;
		DatagramSocket socket;
		// adr contient l'@IP de la partie serveur
		try 
		{
			adr = InetAddress.getByName("scinfe056");
			// données à envoyer : chaîne de caractères
			byte[] data = (new String("youpi")).getBytes();
			// création du paquet avec les données et en précisant l'adresse du serveur
			// (@IP et port sur lequel il écoute : 7777)
			packet = new DatagramPacket(data, data.length, adr, 7777);
			// création d'une socket, sans la lier à un port particulier
			socket = new DatagramSocket();
			socket.send(packet);
			
			// attente paquet envoyé sur la socket du client
			socket.receive(packet);
			// récupération et affichage de la donnée contenue dans le paquet
			String chaine = new String(packet.getData(), 0, 
			packet.getLength());
			System.out.println(" recu du serveur : "+chaine);
		} 
		catch (IOException e)
		{
			e.printStackTrace();
		}

	}

}
