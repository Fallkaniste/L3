import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.lang.reflect.Method;
import java.net.ServerSocket;
import java.net.Socket;

public class Serveur 
{
	private static ServerSocket serverSocket;
	
	public static void main(String[] args) 
	{
		try 
		{
			attendreClient(7777);
		} 
		catch (Exception e) 
		{
			try 
			{
				serverSocket.close();
			} 
			catch (IOException e1) 
			{
				e1.printStackTrace();
			}
			e.printStackTrace();
		}

	}
	
	public static void attendreClient(int port) throws Exception
	{
		serverSocket=new ServerSocket(port);
		DataManager dm = new DataManager();
		
		while(true)
		{
			Socket sock=serverSocket.accept();
			System.out.println("J'ai trouvé un client!");
			(new ThreadClient(sock, dm)).start();
		}
	}
	
	public static void fermerConnexion() throws Exception
	{
		serverSocket.close();
	}

	private static class ThreadClient extends Thread
	{
		Socket sock;
		DataManager dm;
		
		public ThreadClient(Socket sock, DataManager dm)
		{
			super();
			this.sock=sock;
			this.dm=dm;
		}
		
		@Override
		public void run()
		{
			try
			{
				ObjectOutputStream output=new ObjectOutputStream(sock.getOutputStream());
				ObjectInputStream input=new ObjectInputStream(sock.getInputStream());
				
				while(true)
				{
					RequestMessage m = (RequestMessage)input.readObject();
					
					
					Method met = null;
					Object result = null;
					
					try 
					{    
					    met = dm.getClass().getMethod(m.getMethod(), m.getParametersTypes());
					}
					catch (NoSuchMethodException e) 
					{
					    e.printStackTrace();
					}

					try 
					{
					    result =  met.invoke(dm, m.getParameters());
					}
					catch(IllegalAccessException e) 
					{
					    e.printStackTrace();
					}
					catch(InvalidIdException e)
					{
						
					}
					
										
					output.writeObject(new Message(result));
					

					/*else if(m instanceof GetPersonneRequest)
					{
						try
						{
							Personne p = dm.getPersonne(((GetPersonneRequest) m).getID());
							output.writeObject(new PersonneMessage(p));
						}
						catch(InvalidIdException e)
						{
							output.writeObject(new InvalidIdExceptionMessage(e));
						}
					}*/		
				}
			}
			catch(Exception e)
			{
				try 
				{
					sock.close();
				} 
				catch (IOException e1) 
				{
					e1.printStackTrace();
				}
			}
		}
	}
}


