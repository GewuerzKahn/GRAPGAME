#include "Model.h"
#include <sstream>



struct lastColor{
	int id;
	vec3 color;
};

vector<lastColor> vectorUndo;


Model::Model() :
	mIsInitialised(false)
{

}

Model::~Model()
{
	if(mIsInitialised) {
		//Delete any initialised stuff
	}
}


//Loads model information from obj. (simplefied) like described on http://en.wikipedia.org/wiki/Wavefront_.obj_file (25.11.2013; 10:48)
bool Model::loadObjFile(const string &filename){
	int numberOfVertex, numberOfTriangle;
	string line;
	ifstream file(filename.c_str());  

	fileName = filename;

	mIsInitialised = file.good();

	if(mIsInitialised){
		if (file.is_open())
		{
			while ( getline(file,line) )
			{
				if(line.substr(0,2) == "v "){
					stringstream ssin(line.substr(2));
					vec3 tempValue;
					ssin >> tempValue.x >> tempValue.y >> tempValue.z;
					vertices.push_back(tempValue);
				}else if(line.substr(0,2) == "vt"){
					stringstream ssin(line.substr(2));
					vec2 tempValue;
					ssin >> tempValue.x >> tempValue.y ;
					vTextures.push_back(tempValue);
				}else if(line.substr(0,2) == "vn"){
					stringstream ssin(line.substr(2));
					vec3 tempValue;
					ssin >> tempValue.x >> tempValue.y >> tempValue.z;
					normals.push_back(tempValue);
				}else if(line.substr(0,2) == "f "){
					stringstream ssin(line.substr(2));
					vec3 tempValue;
					ssin >> tempValue.x >> tempValue.y >> tempValue.z;
					triangles.push_back(tempValue);
				}

			}

			for(int x=0; x< triangles.size(); x++){
				triangleFaces.push_back(vertices[triangles[x].x - 1]);
				triangleFaces.push_back(vertices[triangles[x].y - 1]);
				triangleFaces.push_back(vertices[triangles[x].z - 1]);
			}
			cout << endl;
			cout << ".obj VERTICES SIZE: " << vertices.size() << endl;
			cout << ".obj VERTICES TEXTURE SIZE: " << vTextures.size() << endl;
			cout << ".obj NORMALS SIZE: " << vTextures.size() << endl;
			cout << ".obj TRIANGLE SIZE: " << triangles.size() << endl;
			cout << ".obj FACES SIZE: " << triangleFaces.size() << endl;

			color = vec3(1.0f, 0.0f, 0.0f);
			setColorVector(color);
			fillUpNormals();
		}
		file.close();
	}
	return mIsInitialised;
}

//Loads model information from .txt file after the assigment 2 file format
bool Model::loadTxtFile(const string &filename)
{
	int numberOfVertex, numberOfTriangle;
	string line;
	ifstream file(filename.c_str());  

	fileName = filename;

	mIsInitialised = file.good();

	if(mIsInitialised){
		file >> numberOfVertex >> numberOfTriangle;
		cout << endl;
		cout << "Filename: " << filename << endl;
		cout << "Vertex: " << numberOfVertex << endl;
		cout << "Triangle: " << numberOfTriangle << endl;

		fillUpVertices(file, numberOfVertex);
		fillUpTriangles( file, numberOfTriangle);
		fillUpNormals();


		fillUpColors(file, numberOfTriangle);



		file.close();
	}
	else{
		cout << "Something with your file is wrong or couldnt be found";
	}

	return mIsInitialised;  //Return true if succusfuly loaded
}

//Save the id as color (id 0 -> RGB(0,0,1); id 1 -> RGB(0,0,2); id 256 -> RGB(0,1,1) etc.)
//The id start from 1, because 0 is the value if the user didn't clicked on the model
vec3 encode_id (int id) {
	id += 1;
	int r = id / 65536;
	r = std::max (r, 0);
	int g = (id - r * 65536) / 256;
	g = std::max (g, 0);
	int b = (id - r * 65536 - g * 256);
	b = std::max (b, 0);

	return vec3 ((float)r / 255.0, (float)g / 255.0, (float)b / 255.0);
}

//Change the color of triangle with the id 
// id -=3 because the array doesnt start from 0
void Model::changeTriangleColor(vec3 color, int id){

	struct lastColor lC = { id, colors[id] };
	cout << "LC ID :" << lC.id << endl;
	vectorUndo.push_back(lC);

	id *= 3;
	id -= 3;
	colors[id] = color;
	colors[id+1] = color;
	colors[id+2] = color;
}


//Change all colors of object
void Model::setColorVector(vec3 color){
	colors.clear();

	for(int x=0; x<triangles.size(); x++){
		addColorToColors(color, x);
	}
}



//Fill the values from the .txt into a vector
void Model::fillUpVertices( ifstream &s, int number){
	vec3 tempValue;

	for(int x=0; x< number; x++){
		s >> tempValue.x >> tempValue.y >> tempValue.z;
		vertices.push_back(tempValue);
	}
}

//Fill the values from the .txt into a vector
void Model::fillUpTriangles( ifstream &s, int number){
	vec3 tempValue;
	for(int x=0; x< number; x++){
		s >> tempValue.x >> tempValue.y >> tempValue.z;
		triangles.push_back(tempValue);

		triangleFaces.push_back(vertices[tempValue.x - 1]);
		triangleFaces.push_back(vertices[tempValue.y - 1]);
		triangleFaces.push_back(vertices[tempValue.z - 1]);
	}
}

//Fill the values for colors from the .txt into colors
void Model::fillUpColors( ifstream &s, int number){
	vec3 tempValue;

	if(!s.eof()){
		for(int x=0; x< number; x++){
			s >> tempValue.x >> tempValue.y >> tempValue.z;
			addColorToColors(vec3(tempValue.x / 255.0,tempValue.y/ 255.0,tempValue.z/ 255.0), x);
		}
	}else{
		color = vec3(1.0f, 0.0f, 0.0f);
		setColorVector(color);
	}
}

void Model::fillUpNormals(){
	vec3 normal;
	for ( int i = 0; i < triangleFaces.size(); i += 3)
	{
		normal = normalize(cross(triangleFaces[i + 1] - triangleFaces[i], triangleFaces[i + 2] - triangleFaces[i]));

		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);

	}
	//cout << "Normals size: " << normals.size() << endl;
}


void Model::draw()
{

	if(!mIsInitialised) { return; }

	glEnable(GL_LIGHTING);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &triangleFaces.front());
	glColorPointer(3, GL_FLOAT, 0, &colors.front());

	glNormalPointer(GL_FLOAT, 0, &normals.front());
	glDrawArrays(GL_TRIANGLES, 0, triangleFaces.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisable(GL_LIGHTING);

}

//"Draw" the object without lighting and with the color ids
void Model::drawPicking()
{
	if(!mIsInitialised) { return; }

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &triangleFaces.front());
	glColorPointer(3, GL_FLOAT, 0, &colorsPicking.front());

	//glNormalPointer(GL_FLOAT, 0, &normals.front());
	glDrawArrays(GL_TRIANGLES, 0, triangleFaces.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

}

//Add triangle color and color id
void Model::addColorToColors(vec3 color, int x){
	colors.push_back(color);
	colors.push_back(color);
	colors.push_back(color);

	colorsPicking.push_back(encode_id(x));
	colorsPicking.push_back(encode_id(x));
	colorsPicking.push_back(encode_id(x));
}

vector<vec3> Model::getColorVector(){
	return colors;
}

void Model::setColorVector(vector<vec3> _colors){
	colors = _colors;
}

int Model::numberOfTriangle(){
	return triangles.size();
}

//Saves the model as .txt as "advanced file format"
void Model::saveTxtFile(){
	ofstream myfile ("save/" + fileName);

	if (myfile.is_open())
	{
		myfile << vertices.size() << " " << triangles.size() << endl;

		for(int index = 0; index < vertices.size(); index++){
			myfile << vertices[index].x << " " << vertices[index].y << " " << vertices[index].z << endl; 
		}
		for(int index = 0; index < triangles.size(); index++){
			myfile << triangles[index].x << " " << triangles[index].y << " " << triangles[index].z << endl; 
		}
		for(int index = 0; index < colors.size(); index += 3){
			myfile << colors[index].x*255.0 << " " << colors[index].y*255.0 << " " << colors[index].z*255.0 << endl; 
		}
		myfile.close();
	}
	else cout << "Unable to open file" << endl; 

}


//Undo the last modified color change
//And I know model shouldn't be responsible for that and I have redudant code, 
//but this function is just an extra
void Model::undo(){

	if(vectorUndo.size() > 0){
		vectorUndo.pop_back();
		cout << "ID " <<vectorUndo.end()->id << endl;

		//TODO REDUDANT CODE (I KNOW!)
		int id = vectorUndo.end()->id;
		id *= 3;
		id -= 3;
		colors[id] = vectorUndo.end()->color;
		colors[id+1] = vectorUndo.end()->color;
		colors[id+2] = vectorUndo.end()->color;
		//changeTriangleColor(vectorUndo.end()->color, vectorUndo.end()->id);

	}else{
		cout << "No undo for you" << endl;
	}
}