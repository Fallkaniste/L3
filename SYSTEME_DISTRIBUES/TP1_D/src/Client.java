import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

import javafx.util.Pair;

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
			Pair<String,Integer> serveur=getOpt(args);
			adr = InetAddress.getByName(serveur.getKey());
			// données à envoyer : chaîne de caractères
			byte[] data = (new String("youpi")).getBytes();
			// création du paquet avec les données et en précisant l'adresse du serveur
			// (@IP et port sur lequel il écoute : 7777)
			packet = new DatagramPacket(data, data.length, adr, serveur.getValue());
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
		catch (Exception e)
		{
			e.printStackTrace();
		}

	}
	
	private static Pair<String,Integer> getOpt(String[] args) throws Exception
	{
		if(args.length!=2 && args.length>0)
		{
			throw new WAException();
		}
		if(args.length==0)
		{
			//Port par défaut
			return new Pair<> ("localhost",7777);
		}
		return new Pair<>(args[0],Integer.parseInt(args[1]));

	}

}
