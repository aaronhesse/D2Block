#include "StdAfx.h"
#include "D2BlockUpdater.h"
#include "D2BlockDownloader.h"

D2BlockUpdater::D2BlockUpdater():
currentRevision(0),
ignoreListOutOfDate(false)
{
}

D2BlockUpdater::~D2BlockUpdater()
{
}

void D2BlockUpdater::UpdateIgnoreList()
{
	if (IgnoreListIsOutOfDate())
		UpdateIgnoreListFile();

	emit UpdaterComplete();
}

void D2BlockUpdater::ProcessRegistryInformation()
{
	QSettings settings("D2Block", "D2Block");

	httpServer = settings.value("Server").toString();
 	updateFile = settings.value("Update File").toString();
 	ignorelistFile = settings.value("Ignorelist File").toString();
 	currentRevision = settings.value("Current Revision").toInt();
}

void D2BlockUpdater::ProcessVersionFile()
{
	url.sprintf("http://%s/%s", httpServer, updateFile);
	
	QByteArray fileData;
	qint64 sizeOfData;

	D2BlockDownloader downloader;
	downloader.DownloadFile(url, &fileData, sizeOfData);

	// parse http data received for revision (line 1)
	// compare our regkey revision to server revision
	// set ignoreListOutOfDate to true or false
}

bool D2BlockUpdater::IgnoreListIsOutOfDate()
{
	ProcessRegistryInformation();
	ProcessVersionFile();

	return ignoreListOutOfDate;
}

void D2BlockUpdater::UpdateIgnoreListFile()
{
#if 0
	// download ignorelist from webserver to d2root/ignorelist.update
	// If we fail to download it, just silently fail. We don't want
	// any of the UI to block or be annoying.
	file = new QFile(updateFile);
	if (!file->open(QIODevice::WriteOnly))
	{
		delete file;
		file = nullptr;
		return;
	}
	// rename user ignorelist 'ignorelist.bak'
	// open ignorelist.bak
	// parse file for "!**D2BLOCK BEGIN**" header
	// save any lines above this.
	// parse file for "!**D2BLOCK END**" footer
	// save any lines below this.
	// append any saved lines to the end of ignorelist.update
	// rename ignorelist.update to 'ignorelist'
	// if everything is successful, set regkey for current revision
#endif
	ignoreListOutOfDate = false;
}
