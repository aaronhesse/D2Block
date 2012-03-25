#include "StdAfx.h"
#include "D2BlockUpdater.h"
#include "D2BlockDownloader.h"

const QString D2BlockUpdater::d2BlockRegPath               = "Software\\D2Block\\";
const QString D2BlockUpdater::d2BlockRegKeyServer          = "Server";
const QString D2BlockUpdater::d2BlockRegKeyUpdateFile      = "Update File";
const QString D2BlockUpdater::d2BlockRegKeyIgnorelistFile  = "Ignorelist File";
const QString D2BlockUpdater::d2BlockRegKeyCurrentRevision = "Current Revision";

D2BlockUpdater::D2BlockUpdater(int argc, char* argv[]):
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
	HKEY key;
	DWORD bytesRead;

	RegOpenKeyEx(HKEY_LOCAL_MACHINE, d2BlockRegPath.toAscii(), 0, KEY_ALL_ACCESS  | KEY_WOW64_32KEY, &key);

	bytesRead = sizeof(httpServer);
	RegGetValue(key, NULL, d2BlockRegKeyServer.toAscii(), RRF_RT_REG_SZ, NULL, httpServer.data(), &bytesRead);

	bytesRead = sizeof(updateFile);
	RegGetValue(key, NULL, d2BlockRegKeyUpdateFile.toAscii(), RRF_RT_REG_SZ, NULL, updateFile.data(), &bytesRead);

	bytesRead = sizeof(ignorelistFile);
	RegGetValue(key, NULL, d2BlockRegKeyIgnorelistFile.toAscii(), RRF_RT_REG_SZ, NULL, ignorelistFile.data(), &bytesRead);

	bytesRead = sizeof(currentRevision);
	RegGetValue(key, NULL, d2BlockRegKeyCurrentRevision.toAscii(), RRF_RT_REG_DWORD, NULL, &currentRevision, &bytesRead);

	RegCloseKey(key);
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
