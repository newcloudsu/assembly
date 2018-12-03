// SP2 Assembler
// 10527225

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>

#include <new>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define SWAP(a,b,t)( (t) = (a), (a) = (b), (b) = (t) )

using namespace std;

struct TableType {
	string TableNum;
	vector<string> TokenType;
};

struct Hash {
	string HashTable;
	bool Is_used = false ;
};

vector<TableType> Table;
vector<string> File;

	vector<Hash> Table5; // symbol
	vector<Hash> Table6; // int
	vector<Hash> Table7; // string
	
	int format[59]= { 3, 3, 2, 3, 2, 3, 3, 2, 3, 3, 2, 1, 1, 1, 3, 3, 3, 3, 3, 3,
						 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 3, 3, 2, 3, 2, 2, 1, 3,
						  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 3, 1, 3, 2, 3 }; // 3 <= 3/4
	
	string Opcode[59] = { "18", "58", "90", "40", "B4", "28", "88", "A0", "24", "64", "9C", "C4", "C0", "F4", "3C",
						 "30", "34", "38", "48", "00", "68", "50", "70", "08", "6C", "74", "04", "D0", "20", "60",
						  "98", "C8", "44", "D8", "AC", "4C", "A4", "A8", "F0", "EC", "0C", "78", "54", "80", "D4",
						  "14", "7C", "E8", "84", "10", "1C", "5C", "94", "B0", "E0", "F8", "2C", "B8", "DC" };
	
	int x, y; // axis
	vector<int> x_axis;
	vector<int> y_axis;
	vector<int> CurLineTokenNum;
	
	int HashValue( string str );
	bool WhiteSpace ( char ch );

	bool StringIsWhiteSpace( string str ) { // ex: "   \n"
		for ( int i = 0; i < str.size(); i++ ) {
			if ( !WhiteSpace( str.at(i) ) ) {
				//cout << "this string is not all whitespace\n";
				return false;
			} // if
			
		} // for
		
		return true;
	} // StringIsWhiteSpace()
	
	void ReadFileAgain( string fileName ) {
		
		fstream pFile; // declear
		string fileString;
		fileName =  fileName + ".txt";
	
		pFile.open( fileName.c_str() , ios::in ); // open the file
		if ( !pFile.is_open() ) {
  	    	cout << endl << "### " << fileName << " does not exist! ###" << endl;
  	  	} // if
  	  	else {
  	  		while ( getline( pFile, fileString, '\n' ) ) {
  	  			//if ( ( fileString != "" ) && !StringIsWhiteSpace( fileString ) ) { // 
  	  				File.push_back( fileString );
				//} // if
  	  			
			} // while
  	  		
		} // else
		
		pFile.close();
		
	} // ReadFileAgain()


	bool IsDelimiter ( char ch ) {
		string str ;
		str = str + ch;
		//cout << str << " :str\n";
		for ( int j = 0; j < Table[3].TokenType.size(); j++ ) {
			if( str == Table[3].TokenType[j] ) {
				return true;
			} // if
			
		} // for
		
		return false;
	} // IsDelimiter()

	bool WhiteSpace ( char ch ) {
		if ( ch == ' ' || ch == '\t' || ch == '\n' )
			return true;
			
		return false;
		
	} // WhiteSpace ()

	bool IsInTable ( string str ) {
		
		for ( int i = 0; i < Table.size(); i++ ) {			
			for ( int j = 0; j < Table[i].TokenType.size() ; j++ ) {
				if ( stricmp( str.c_str(), Table[i].TokenType[j].c_str() ) == 0 ) { // ignore case
					
					cout << "(" << i+1 << "," << j+1 << ")\n" ;
					x = i+1;
					y = j+1;
					x_axis.push_back( x );
					y_axis.push_back( y );
					return true;
				} // if
			} // for
		} // for
		
		return false;
		
	} // IsInTable()
	

	
	bool IsDigit( string str ) {
		
		if ( str == "" ) return false;
		
		char check;
		int num = 0;
		for( int i = 0; i < str.size() ; i++ ) {
			
			check = str.at(i);
			if ( ( check <= 57 && check >= 48 ) ) { // 0~9 // A~F || ( check >= 65 && check <= 70 ) 
				num++;
			} // if
			
		} // for
		
		if ( num == str.size() && num != 0 ) {
			
			//cout << str << " is digit\n";
			return true;
		} // if
		
		return false;
			
	} // IsDigit()
	
	bool IsString( string str ) {
		
		char check;
		int num = 0;
		for( int i = 0; i < str.size() ; i++ ) {
			
			check = str.at(i);
			if ( ( check == '"' ) ) { // 0~9, A~F
				num++;
			} // if
			
		} // for
		
		if ( num == 2 ) return true;
		return false;		
		
	} // IsString()
	
	bool StringInSpecial_Case( string str ) {
		int index = HashValue( str );
		while ( Table7[index].Is_used ) {
			index++;
		} // while			

		Table7[index].HashTable = str;
		Table7[index].Is_used = true;
		x = 7;
		y = index;
		x_axis.push_back( x );
		y_axis.push_back( y );
		
		cout << "(" << x << "," << y << ")\n" ;
		
	} // StringInSpecial_Case()

	bool IntInHex_Case( string str ) { // hex
		int index = HashValue( str );
		while ( Table6[index].Is_used ) {
			index++;
		} // while			

		Table6[index].HashTable = str;
		Table6[index].Is_used = true;
		x = 6;
		y = index;
		x_axis.push_back( x );
		y_axis.push_back( y );
		
		cout << "(" << x << "," << y << ")\n" ;
		
	} // StringInSpecial_Case()
		

	void CreateNewTable () { 
	
		for ( int i = 0; i < 100 ; i++ ) {
			Hash temp;
			Table5.push_back( temp );
			Table6.push_back( temp );
			Table7.push_back( temp );			
		} // for		
		
	} // CreateNewTable()


	int HashValue( string str ) {
		
		char ch;
		int num = 0, total = 0;
		for( int i = 0; i < str.size() ; i++ ) {
			ch = str.at(i);	
			total = total + ch;
			//printf( "ch is %c : %d\n", ch , ch );
		} // for
		
		total = total % 100;
		return total;		
		
	} // HashValue()


	void PutTokenInTable ( string str ) { 
		
		int index = HashValue( str );

		if ( IsDigit( str ) ) { // int
		
			while ( Table6[index].Is_used ) { // collision
				index++;
			} // while
			
			Table6[index].HashTable = str;
			Table6[index].Is_used = true;
			x = 6;
			y = index;
			x_axis.push_back( x );
			y_axis.push_back( y );
				
		} // if
		else if ( IsString( str ) ) { // string
		
			while ( Table7[index].Is_used ) {
				index++;
			} // while			

			Table7[index].HashTable = str;
			Table7[index].Is_used = true;
			x = 7;
			y = index;
			x_axis.push_back( x );
			y_axis.push_back( y );
									
		} // else if
		else { // symbol
		
			while ( Table5[index].Is_used ) {
				if ( stricmp( str.c_str(), Table5[index].HashTable.c_str() ) == 0 ) {
					break;	
				} // if
				else {
					index++;
				} // else
				
			} // while	
					
			Table5[index].HashTable = str;
			Table5[index].Is_used = true;
			x = 5;
			y = index;
			x_axis.push_back( x );
			y_axis.push_back( y );
						
		} // else
		
		cout << "(" << x << "," << y << ")\n" ;
		
	} // PutTokenInTable()

	void loadfile ( string fileName ) { // load a file

		fstream pFile; // declear
	
		fileName =  fileName + ".txt";
	
		pFile.open( fileName.c_str() , ios::in ); // open the file
		if ( !pFile.is_open() ) {
  	    	cout << endl << "### " << fileName << " does not exist! ###" << endl;
  	  	} // if
   	 	else {
			
			char ch , reg;
			string token;
			int LineTokenNum = 0; // the amount of token in this line
			pFile.get(ch);
			/*
			while ( WhiteSpace( ch ) ) {
				pFile.get(ch);
			} // while
			*/
			while ( !pFile.eof() ) {
				
				while ( ( !WhiteSpace( ch ) && !IsDelimiter( ch ) )  ) { // || ( ch == '\'' )
					
					token = token + ch;
					//if ( ch == '\'' ) break;
					
					reg = ch; // pre char
					pFile.get(ch);
					
				} // while
				
				while ( IsDelimiter( ch ) && ch != '.' ) { // will have two token
					
					if ( ch == '\'' || reg == '\n' )
					 token = token + ch;
					
					// process token1
					if ( token != "" ) {
						cout << token << " :token\n";
						LineTokenNum++;
						
						if ( IsInTable( token ) ) {
						} // if
						else { // symbol , int , string
							PutTokenInTable ( token ) ;						
						} // else
						
					} // if
					
					
					//process token2
					token = "";
					if( ch != '\'')
						token = token + ch;
					
					reg = ch;
					pFile.get(ch);
					if ( ch == '\n' ) break;
					/*
					cout << token << " :token\n";
					LineTokenNum++;
					if ( IsInTable( token ) ) {
					} // if
					else { // symbol , int , string
						PutTokenInTable ( token ) ;						
					} // else
					*/
					
				} // while
			
				if ( token != "" ) {
				
					cout << token << " :token\n";
					LineTokenNum++;

					if ( IsInTable( token ) ) {
					} // if
					else { // symbol , int , string
						PutTokenInTable ( token ) ;						
					} // else
					
				} // if
				
				if ( ch == '\n'  ) { // except '\n' '\n'&& token != ""
					//if ( reg != '\'' ) {
						cout << LineTokenNum << " :LineTokenNum\n";
						CurLineTokenNum.push_back( LineTokenNum ); // count how many tokens in this line
						LineTokenNum = 0;	
					//} // if	
				} // if
				
					
				token = ""; // initialize
				
				reg = ch;
				while ( WhiteSpace( ch ) && !pFile.eof() ) {
					pFile.get(ch);
					if( ch == '\n' ) {
						cout << LineTokenNum << " :LineTokenNum\n";
						CurLineTokenNum.push_back( LineTokenNum ); // count how many tokens in this line
						LineTokenNum = 0;
					} // if
					
					reg = ch;
					cout << "read\n";
				} // while
				
				
				if ( ch == 'X' ) { // hex
				
					pFile.get(ch);
					if ( ch == '\'' ) {
						
						token = token + ch;
						cout << token << " :token\n";
						LineTokenNum++;
						IsInTable( token ); // debug
						token = "";

						pFile.get(ch);	
						while ( ch != '\'') {
							token = token + ch;
							reg = ch;
							pFile.get(ch);	
						} // while
						
						IntInHex_Case ( token );
						cout << token << ":int token\n";
						LineTokenNum++;
						system("pause");
						token = "";

					} // if
					else {
						token = token + reg; // ex : CLOOP 還原'C' 
					} // else							
					
				} // if
				else if ( ch == 'C' ) { // string
				
					pFile.get(ch);
					if ( ch == '\'') {
						
						// process '\''
						token = token + ch;
						cout << token << " :token\n";
						LineTokenNum++;
						IsInTable( token ); // debug
						token = "";
						
						reg = ch;
						pFile.get(ch);
						while ( ch != '\'') {
							token = token + ch;
							reg = ch;
							pFile.get(ch);
						} // while
						
						StringInSpecial_Case ( token );
						cout << token << ":string token\n";
						LineTokenNum++;
						system("pause");
						token = "";
						
					} // if
					else {
						token = token + reg; // ex : CLOOP 還原'C' 
					} // else

				} // else if
				else if( ch == '.' ) { // comment
					
					token = "";
					token = token + ch; 
					IsInTable( token ); // process '.'
					LineTokenNum++;
					
					pFile.get(ch);
					while ( ch != '\n') {
						reg = ch;
						pFile.get(ch);
					} // while
					
					token = "";
					
				} // else if
	
			} // while
			
		} // else
	
		pFile.close();
		
	} // loadfile()
	
	void loadtable( string TableName ) {
	 	
	 	fstream pFile; // declear
	 	string str;
	 	string token = "";
	 	TableType temp;
	 	temp.TableNum = TableName;
		TableName = TableName + ".table"; 
		pFile.open( TableName.c_str() , ios::in ); // open the file
		
		while ( getline( pFile, str, '\n' ) ) {
			//cout << token << " -> token\n";
			for ( int i = 0; i < str.size(); i++ ) {
				if( !WhiteSpace( str.at(i) ) ) {
					token = token + str.at(i);
				} // if
			} // for
			
			temp.TokenType.push_back( token );
			token = "";
		} // while
		
	 	Table.push_back( temp );
	 	
	} // loadtable()
	
	
	void Print() {
		for ( int i = 0; i < Table.size(); i++ ) {
			
			cout << Table[i].TableNum << "\n" ;
			cout << Table[i].TokenType.size() << " ->size\n";
			
			for ( int j = 0; j < Table[i].TokenType.size() ; j++ ) {
				cout << Table[i].TokenType[j] << "\n" ;
			} // for
			
		} // for
	} // Print()
	
	
	void PrintOutFile() {
		
		int total = -1;
		for( int i = 0; i < File.size(); i++ ) {  // File.size() == CurLineTokenNum.size()
		
			printf( "%s\r\n", File[i].c_str() );
			//system("pause");
			total++;
			int j = 0;
			while ( j < CurLineTokenNum[i] ) {	
				printf( "(%d,%d)", x_axis[ total ], y_axis[ total ] );
				//cout << "( " << x_axis[ total ] << "," <<  y_axis[ total ] << " )";
				j++;
				total++ ;
			} // for

			total--;
			printf( "\r\n" );
			system("pause");
			
		} // for
			
	} // Print()	
	
int main(int argc, const char * argv[]) {
	
	string fileName;
	Table.clear();
	 
	cout << "*****************************************\n" ; 
    cout << " Assembler               ****************\n" ;
    cout << "*****************************************\n" ; 
	cout << "Input the file number:  [0]Quit\n" ;
				
		
	cin >> fileName;
	getchar();
	while ( fileName != "0" ) {
		
		loadtable( "Table1" );
		loadtable( "Table2" );
		loadtable( "Table3" );
		loadtable( "Table4" );
		
		CreateNewTable(); //  Symbol:Table5 , Integer :Table6 , String :Table7 ;
	
		loadfile( fileName );
	
		ReadFileAgain( fileName );
		
		
		for ( int i = 0; i < CurLineTokenNum.size(); i++ ) {
			cout << CurLineTokenNum[i] << " :num of\n";
		} // for
		/*
		for ( int j = 0; j < x_axis.size(); j++ ) {
			cout << "(" << x_axis[j] << "," << y_axis[j] << ")" << " :axis\n";
		} // for
		*/
		
		cout << "output file is:\n";
		PrintOutFile();
		system("pause");
		
		/*
		fileName = fileName + "_output.txt";
	
		int total = -1;
		FILE *outfile = NULL;
		outfile = fopen( fileName.c_str(), "wb" );
		
		if ( outfile != NULL ) {
			
			for( int index = 0; index < File.size(); index++ ) {  // File.size() == CurLineTokenNum.size()
			
				total++;
				fprintf( outfile, "%s\r\n", File[index].c_str() );
				for ( int j = 0; j < CurLineTokenNum[index] ; j++ ) {
					fprintf( outfile, "(%d,%d)", x_axis[ total ], y_axis[ total ] );
					total++;
				} // for
					
				total--;
				fprintf( outfile, "\r\n" );
				
			} // for
		} // if
		else {
			cout << "open file error!\n";
		} //else
		
		fclose( outfile );
		*/
		
		Table.clear();
		Table5.clear();
		Table6.clear();
		Table7.clear();
		File.clear();
		x_axis.clear();
		y_axis.clear();
		CurLineTokenNum.clear();
	
		cout << "Input the file number:  [0]Quit\n" ;
		 
		cin >> fileName;
		getchar();
			
	} // while
	
} // main()
