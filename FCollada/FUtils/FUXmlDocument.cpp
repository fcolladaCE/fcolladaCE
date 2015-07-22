/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America
	
	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

#include "StdAfx.h"
#include "FUXmlDocument.h"
#include "FUXmlWriter.h"
#include "FUFileManager.h"
#include "FUFile.h"
#include "FCDocument/FCDocument.h"

#define MAX_FILE_SIZE 10240000
//
// FUXmlDocument
// 

FUXmlDocument::FUXmlDocument(FUFileManager* manager, const fchar* _filename, bool _isParsing)
:	isParsing(_isParsing), filename(_filename)
,	xmlDocument(NULL)
{
	if (isParsing)
	{
#if 0
		FUFile* file = NULL;
		if (manager != NULL) file = manager->OpenFile(filename, FUFile::READ);
		else file = new FUFile(filename, FUFile::READ);

		if (file->IsOpen())
		{
			size_t fileLength = file->GetLength();
			uint8* fileData = new uint8[fileLength];
			file->Read(fileData, fileLength);
			file->Close();

			// Open the given XML file.
			xmlDocument = xmlParseMemory((const char*) fileData, (int)fileLength);
			SAFE_DELETE_ARRAY(fileData);
		}
		SAFE_DELETE(file);
#else
		const fm::string filenameUtf8 = FUStringConversion::ToString(_filename);
		//push 1kb of data at a time
		FILE *f;
#if defined(_MSC_VER) && defined(UNICODE)
		// On Windows fopen does not support UTF-8 and will always instead use the current local codepage.
		// We use _wfopen instead to circumvent that limitation, since this one does use UCS-2 aka UTF-16 and thus supports all legal filenames on all systems
		f = _wfopen(_filename, L"r");
#else
		f = fopen(filenameUtf8, "r");
#endif
    	if (f != NULL) 
		{
      		int res, size = 1024;
      		char chars[1024];
      		xmlParserCtxtPtr ctxt;

      		res = fread(chars, 1, 4, f);
	      	if (res > 0) 
			{
    		    ctxt = xmlCreatePushParserCtxt(NULL, NULL, chars, res, filenameUtf8);
    		    while ((res = fread(chars, 1, size, f)) > 0) 
				{
    		      xmlParseChunk(ctxt, chars, res, 0);
    		    }
    		    xmlParseChunk(ctxt, chars, 0, 1);
    		    xmlDocument = ctxt->myDoc;
    		    xmlFreeParserCtxt(ctxt);
			}

			fclose(f);
    	}
#endif
	}
	else
	{
		xmlDocument = xmlNewDoc(NULL); // NULL implies version 1.0.
	}
}

FUXmlDocument::FUXmlDocument(const char* data, size_t length)
:	isParsing(true)
,	xmlDocument(NULL)
{
	FUAssert(data != NULL, return);

	if (length == (size_t) ~0)
	{
		const char* c = data;
		for (length = 0; length < MAX_FILE_SIZE; ++length)
		{
			if (*(c++) == 0) break;
		}
		FUAssert(length < MAX_FILE_SIZE, return);
	}

	// Open the given XML file.
	xmlDocument = xmlParseMemory(data, (int)length);
}

FUXmlDocument::~FUXmlDocument()
{
	// Release the XML document
	ReleaseXmlData();
}

xmlNode* FUXmlDocument::GetRootNode()
{
	if (xmlDocument == NULL)
	{
		return NULL;
	}

	return xmlDocGetRootElement(xmlDocument);
}

xmlNode* FUXmlDocument::CreateRootNode(const char* name)
{
	xmlNode* rootNode = NULL;
	if (!isParsing)
	{
		if (xmlDocument == NULL) xmlDocument = xmlNewDoc(NULL); // NULL implies version 1.0.
		rootNode = FUXmlWriter::CreateNode(name);
		xmlDocSetRootElement(xmlDocument, rootNode);
	}
	return rootNode;
}

void FUXmlDocument::ReleaseXmlData()
{
	if (xmlDocument != NULL)
	{
		if (!isParsing)
		{
		}
		xmlFreeDoc(xmlDocument);
		xmlDocument = NULL;
	}
}

// Writes out the XML document.
bool FUXmlDocument::Write(const char* encoding)
{
	FUFile file(filename, FUFile::WRITE);
	if (!file.IsOpen()) return false;
	xmlDocument->encoding = xmlStrdup((const xmlChar*) encoding);
	return xmlDocFormatDump(file.GetHandle(), xmlDocument, 1) > 0;
}
