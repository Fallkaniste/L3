import java.io.EOFException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.lang.reflect.Method;
import java.net.InetAddress;
import java.net.Socket;

public class Client 
{
	private static Socket sock;
	private static ObjectOutputStream output;
	private static ObjectInputStream input;
	private static String metName;
	@SuppressWarnings("rawtypes")
	private static Class[] paramTypes;
	private static Object[] param;
	
	public static void main(String[] args) 
	{
		try 
		{
			ouvrirConnexion("localhost", 7777);
			int id=(int)addPersonne(new Personne("Roger",54));
			System.out.println("Roger a bien été ajouté avec l'id :"+id);
			int id2=(int)addPersonne(new Personne("Jean-Pierre",62));
			System.out.println("Jean-Pierre a bien été ajouté avec l'id :"+id2);
			/*System.out.println("La personne avec l'id "+id+" est "+getPersonne(id));
			System.out.println("La personne avec l'id "+id2+" est "+getPersonne(id2));
			System.out.println("Roger est à l'id "+getID(new Personne("Roger",54)));*/
			
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
	
	@SuppressWarnings("unchecked")
	public static Object addPersonne(Personne p) throws Exception
	{
		metName="addPersonne";
		
		paramTypes=new Class[1];
		paramTypes[0]=p.getClass();
		
		param=new Object[1];		
		param[0]=p;
			
		output.writeObject(new RequestMessage(metName,paramTypes, param));
		
		Object o = input.readObject();
		return ((Message)o).getValue();
	}
	
	/*public static int getID(Personne p) throws Exception
	{
		output.writeObject(new GetIdRequest(p));
		
		try
		{
			Object o = input.readObject();
			System.out.println("Je suis arrivé là");
			if(o instanceof IdMessage)
			{
				return ((IdMessage)o).getID();
			}
		}
		catch (EOFException e)
		{
			e.printStackTrace();
		}


		
		throw new Exception("Message non attendu");
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
	}*/
	
	public static void fermerConnexion() throws Exception
	{
		input.close();
		output.close();
		sock.close();
	}

}
