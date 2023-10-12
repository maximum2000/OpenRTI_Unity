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

		//string allxml = File.ReadAllText(Application.streamingAssetsPath + "/fdd_test.xml");
		//allxml = allxml.Replace("xmlns = \"http://standards.ieee.org/IEEE1516-2010\"" , "");
		//allxml = allxml.Replace("xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"", "");
		//allxml = allxml.Replace("xsi:schemaLocation=\"http://standards.ieee.org/IEEE1516-2010 http://standards.ieee.org/downloads/1516/1516.2-2010/IEEE1516-DIF-2010.xsd\"", "");

		try
		{
			//xmlDoc = XDocument.Load(new StringReader(allxml));
			xmlDoc = XDocument.Load(Application.streamingAssetsPath + "/fdd_test.xml");
		}
		catch (FileNotFoundException)
		{
			// The underlying file of the path cannot be found
			Debug.Log("XDocument.Load failed!");
			return;
		}

		//элементы матмодели
		XElement _rootElements = xmlDoc.Root; //.Element("objectModel");//.Element("objects").Element("objectClass"); //HLAobjectRoot
		foreach (XElement element_of_objects in _rootElements.Elements())
        {
			Debug.Log("read new with name = " + element_of_objects.Name); //ToString()
			Debug.Log("read new with name = " + element_of_objects.Value); //ToString()
		}

		return;
		foreach (XElement el in _rootElements.Elements())
		{
			if (el.Name == "objectClass")
			{
				string objectname = el.Element("name").Value;
				Debug.Log("read new objectClass with name = " + objectname);

				foreach (XElement attr in el.Elements())
				{
					if (attr.Name == "attribute")
					{
						string attribute_name = attr.Element("name").Value;
						Debug.Log("read new attribute = " + "(O)"+objectname  + "." + "(O)"+ attribute_name);
					}
				}

				Debug.Log("end read new objectClass with name = " + name);
			}
		}
		//end for
	}
	//

}





