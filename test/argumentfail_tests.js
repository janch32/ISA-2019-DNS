module.exports = [
	// Moc argumentů
	[`-s8.8.8.8`, `ahoj`, `svete`], 
	
	// Nespecifikovaná dotazovaná adresa
	[`-s8.8.8.8`], 
	
	// Neznámé flagy
	[`-s8.8.8.8`, `-qwertzuiop`, `vutbr.cz`], 
	
	// Více typů dotazů
	[`-s8.8.8.8`, `-6c`, `vutbr.cz`], 
	
	// vutbr.cz není validní IP adresa (reverzní záznam)
	[`-s8.8.8.8`, `-x`, `vutbr.cz`], 
	
	// Chybí parametr flagu
	[`-s8.8.8.8`, `-x`, `vutbr.cz`, `-p`], 
	
	// Nevalidní IPv6 adresa serveru
	[`-s80d0::45fe:dqqq`, `vutbr.cz`] 
]
