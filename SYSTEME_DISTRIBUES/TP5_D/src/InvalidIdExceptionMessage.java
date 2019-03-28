
public class InvalidIdExceptionMessage extends Message 
{

	private static final long serialVersionUID = -1575101283482606944L;
	InvalidIdException e;
	
	public InvalidIdExceptionMessage()
	{
		this.e=new InvalidIdException();
	}
	
	public InvalidIdExceptionMessage(InvalidIdException e)
	{
		this.e=e;
	}
	
	public InvalidIdException getException()
	{
		return this.e;
	}
	
	
}
