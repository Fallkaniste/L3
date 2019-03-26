import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.Socket;

public class Client 
{
	private static Socket sock;
	private static ObjectOutputStream output;
	private static ObjectInputStream input;
	
	public static void main(String[] args) 
	{
		try 
		{
			ouvrirConnexion("localhost", 7777);
			int id=addPersonne(new Personne("Roger"));
			System.out.println("Roger a bien été ajouté avec l'id :"+id);
			int id2=addPersonne(new Personne("Jean-Pierre"));
			System.out.println("Jean-Pierre a bien été ajouté avec l'id :"+id2);
			System.out.println("La personne avec l'id "+id+" est "+getPersonne(id));
			System.out.println("La personne avec l'id "+id2+" est "+getPersonne(id));
			
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}
	}
	
	public static void ouvrirConnexion(String host, int port) throws Exception
	{
		InetAddress adr = InetAddress.getByName(host);
		sock=new Socket(adr, port);
		output=new ObjectOutputStream(sock.getOutputStream());
		input=new ObjectInputStream(sock.getInputStream());
	}
	
	public static int addPersonne(Personne p) throws Exception
	{
		output.writeObject(new AddPersonneRequest(p));
		
		Object o = input.readObject();
		
		if(o instanceof IdMessage)
		{
			return ((IdMessage)o).getID();
		}
		
		throw new Exception("Message non attendu"+o);
	}
	
	public static Personne getPersonne(int id) throws Exception
	{
		output.writeObject(new GetPersonneRequest(id));
		Message m = (Message) input.readObject();
		
		if(m instanceof PersonneMessage)
		{
			return ((PersonneMessage) m).getPersonne();
		}
		if(m instanceof InvalidIdExceptionMessage)
		{
			throw ((InvalidIdExceptionMessage) m).getException();
		}
		throw new Exception("Message non attendu"+m);
	}
	
	public static void fermerConnexion() throws Exception
	{
		input.close();
		output.close();
		sock.close();
	}

}
