import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class Serveur 
{

	public static void main(String[] args) 
	{	
		DatagramPacket packet;
		DatagramSocket socket;

		try 
		{
			socket = new DatagramSocket(getOpt(args));
			
			// tableau de 15 octets qui contiendra les données reçues
			byte[] data = new byte[15];
			// création d'un paquet en utilisant le tableau d'octets
			packet = new DatagramPacket(data, data.length);
			// attente de la réception d'un paquet. Le paquet reçu est placé dans
			// packet et ses données dans data.
			socket.receive(packet);
			// récupération et affichage des données (une chaîne de caractères)
			String chaine = new String(packet.getData(), 0,
			packet.getLength()); 
			System.out.println(" recu : "+chaine);
			System.out.println(" ca vient de : "+
			packet.getAddress()+":"+ packet.getPort());
			// on met une nouvelle donnée dans le paquet 
			// (qui contient donc le couple @IP/port de la socket coté client)
			String reponse ="bien recu";
			packet.setData(reponse.getBytes());
			packet.setLength(reponse.length());
			// on envoie le paquet au client
			socket.send(packet);
		} 
		catch (Exception e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
	
	private static int getOpt(String[] args) throws Exception
	{
		if(args.length>1)
		{
			throw new WAException();
		}
		if(args.length==0)
		{
			//Port par défaut
			return 7777;
		}
		return Integer.parseInt(args[0]);

	}

}
