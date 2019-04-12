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
			System.out.println("kjdegjed");
			int id=(int)makeRequest(new Personne("Roger",54), "addPersonne");
			System.out.println("Roger a bien été ajouté avec l'id :"+id);
			int id2=(int)makeRequest(new Personne("Jean-Pierre",62), "addPersonne");
			System.out.println("Jean-Pierre a bien été ajouté avec l'id :"+id2);
			System.out.println("La personne avec l'id "+id+" est "+(Personne)makeRequest(id, "getPersonne"));
			System.out.println("La personne avec l'id "+id2+" est "+(Personne)makeRequest(id2, "getPersonne"));
			System.out.println("Roger est à l'id "+(int)makeRequest(new Personne("Roger",54), "getID"));
			
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
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static Object makeRequest(Object p, String metName) throws Exception
	{		
		Class[] paramTypes=new Class[1];
		paramTypes[0]=p.getClass();
		
		Object[] param=new Object[1];		
		param[0]=p;
			
		output.writeObject(new RequestMessage(metName,paramTypes, param));
		
		Object o = input.readObject();
		return ((Message)o).getValue();
	}
	
	public static void fermerConnexion() throws Exception
	{
		input.close();
		output.close();
		sock.close();
	}

}
