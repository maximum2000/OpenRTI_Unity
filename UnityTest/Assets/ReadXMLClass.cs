/***************************************************************************
ReadXMLClass.cs  - Загрузка файла описания федерации
-------------------
begin                : 12 октября 2023
copyright            : (C) 2023 by Гаммер Максим Дмитриевич (maximum2000)
email                : Maxim.Gammer@yandex.ru
site				 : lcontent.ru 
org					 : Гаммер Максим Дмитриевич
***************************************************************************/

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using System.IO;
using System.Xml.Linq;
using UnityEngine.UI;
using System.Globalization;



public class ReadXMLClass : MonoBehaviour
{
	XDocument xmlDoc;
	//ММ
	public Dictionary<string, string> Etalons = new Dictionary<string, string>();

	void Start()
	{
		//DontDestroyOnLoad(gameObject);
		LoadXML();
	}

	void LoadXML()
	{
		System.Globalization.CultureInfo customCulture = (System.Globalization.CultureInfo)System.Threading.Thread.CurrentThread.CurrentCulture.Clone();
		customCulture.NumberFormat.NumberDecimalSeparator = ".";
		System.Threading.Thread.CurrentThread.CurrentCulture = customCulture;

		string allxml = "";
		try
		{
			allxml = File.ReadAllText(Application.streamingAssetsPath + "/fdd_test.xml");
		}
		catch (FileNotFoundException)
		{
			Debug.Log("fdd_test.xml  FileNotFoundException !");
			return;
		}

		allxml = allxml.Replace("http://standards.ieee.org/IEEE1516-2010", "");
		//Debug.Log(allxml);

		
		xmlDoc = XDocument.Load(new StringReader(allxml));
		//xmlDoc = XDocument.Load(Application.streamingAssetsPath + "/fdd_test.xml");
		

		XElement _rootObjects = xmlDoc.Root.Element("objects").Element("objectClass");
		foreach (XElement object_in_XML in _rootObjects.Elements("objectClass"))
		{
			string s1 = object_in_XML.Element("name").Value.ToString();
			Debug.Log("new object = " + s1);
			foreach (XElement attribute_in_object in object_in_XML.Elements("attribute"))
			{
				string s2 = attribute_in_object.Element("name").Value;
				Debug.Log("new attribute = " + s1 + "." + s2);
			}
		}

		XElement _rootInteractions = xmlDoc.Root.Element("interactions").Element("interactionClass");
		foreach (XElement interaction_in_XML in _rootInteractions.Elements("interactionClass"))
		{
			string s1 = interaction_in_XML.Element("name").Value.ToString();
			Debug.Log("new interaction = " + s1);
			foreach (XElement parameter_in_object in interaction_in_XML.Elements("parameter"))
			{
				string s2 = parameter_in_object.Element("name").Value;
				Debug.Log("new parameter = " + s1 + "." + s2);
			}
		}

	}
		//

}





