import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Serveur 
{

	public static void main(String[] args) 
	{
		int port=7777;
		
		if (args.length!=1)
		{
			System.out.println("Vous devez entrer le port.");
			System.exit(-1);
		}
		else
		{
			port=Integer.parseInt(args[0]);
		}
		
		try 
		{
			// serveur positionne sa socket d'écoute sur le port local 7777
			ServerSocket serverSocket = new ServerSocket(port);
			
			// se met en attente de connexion de la part d'un client distant
			Socket socket = serverSocket.accept();
			
			// connexion acceptée : récupère les flux objets pour communiquer
			// avec le client qui vient de se connecter
			ObjectOutputStream output = new ObjectOutputStream(socket.getOutputStream());
			ObjectInputStream input = new ObjectInputStream(socket.getInputStream());
			
			// attente les données venant du client
			String chaine = (String)input.readObject();
			System.out.println(" recu : "+chaine);
			
			// affiche les coordonnées du client qui vient de se connecter
			System.out.println(" ca vient de : "+socket.getInetAddress()+":"+socket.getPort());
			
			// envoi d'une réponse au client
			output.writeObject(new String("bien recu"));
			
			serverSocket.close();
		} 
		catch (IOException | ClassNotFoundException e) 
		{
			e.printStackTrace();
		}
	}

}
