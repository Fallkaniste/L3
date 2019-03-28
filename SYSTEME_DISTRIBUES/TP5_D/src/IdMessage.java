
public class IdMessage extends Message 
{
	private static final long serialVersionUID = 3597923772169392267L;
	private int id;
	
	public IdMessage()
	{
		this.id=0;
	}
	
	public IdMessage(int id)
	{
		this.id=id;
	}
	
	public void setID(int id)
	{
		this.id=id;
	}
	
	public int getID()
	{
		return this.id;
	}
}
