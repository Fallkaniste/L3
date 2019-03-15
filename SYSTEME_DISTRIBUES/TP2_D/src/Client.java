import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.Socket;

public class Client 
{
	public static void main(String[] args) 
	{
		String hostName="localhost";
		int port=7777;
		
		if (args.length!=2)
		{
			System.out.println("Vous devez entrer le nom d'hôte et le port (dans cet ordre).");
			System.exit(-1);
		}
		else
		{
			hostName=args[0];
			port=Integer.parseInt(args[1]);
		}
		
		try 
		{
			// adresse IP du serveur
			InetAddress adr;

			adr = InetAddress.getByName(hostName);
		
			// ouverture de connexion avec le serveur sur le port 7777
			Socket socket = new Socket(adr, port);
			
			// construction de flux objets à partir des flux de la socket
			ObjectOutputStream output = new ObjectOutputStream(socket.getOutputStream());
			ObjectInputStream input = new ObjectInputStream(socket.getInputStream());
			
			// écriture d'une chaîne dans le flux de sortie : c'est-à-dire envoi de
			// données au serveur
			output.writeObject(new String("youpi"));
			
			// attente de réception de données venant du serveur (avec le readObject)
			// on sait qu'on attend une chaîne, on peut donc faire un cast directement
			String chaine = (String)input.readObject();
			System.out.println(" recu du serveur : "+chaine);
			
			socket.close();
		} 
		catch (IOException | ClassNotFoundException e) 
		{
			e.printStackTrace();
		}
		
	}

}
