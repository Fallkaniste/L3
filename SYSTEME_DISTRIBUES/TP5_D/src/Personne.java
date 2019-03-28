import java.io.Serializable;

public class Personne implements Serializable
{
	private static final long serialVersionUID = 4595136155849349322L;
	private String name;
	private int age;
	
	public Personne()
	{
		this.name="defaultName";
		this.age=0;
	}
	
	public Personne(String name, int age)
	{
		this.name=name;
		this.age=age;
	}
	
	@Override
	public String toString()
	{
		return this.name+", "+age+" ans";
	}
	
    public boolean equals(Object obj)
    {
	if (obj == null) return false;
	if (!(obj instanceof Personne)) return false;
	
	Personne p = (Personne)obj;
	return ((age == p.age) && (name.equals(p.name)));
    }
}
