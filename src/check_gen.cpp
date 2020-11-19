#include <fstream>

int main(int argc, char ** argv){
	if(argc < 8){
		printf("\nUSAGE: \"rosrun pattern_generator check_gen L R B T s e name\"\n");
		
		printf("\tL:\tLeft bound of the target (center is 0,0).\n");
		printf("\tR:\tRight bound of the target (center is 0,0).\n");
		printf("\tB:\tLower bound of the target (center is 0,0).\n");
		printf("\tT:\tUpper bound of the target (center is 0,0).\n");
		printf("\ts:\tSize of a square (edge to edge).\n");
		printf("\tname:\tName of the model produced.\n");
		printf("\te:\tAdditional white border surrounding targer (adds onto LRTB).\n");
		
		printf("\n(All values should be in m.)\n\n");
		
		return 0;
	}
	
	double l = std::stod(argv[1]);
	double r = std::stod(argv[2]);
	double b = std::stod(argv[3]);
	double t = std::stod(argv[4]);
	double s = std::stod(argv[5]);
	double e = std::stod(argv[6]);
	char * name = argv[7];
	
	if(l > r){
		printf("Left (%f) is greater than right (%f).\n", l, r);
		return 0;
	}
	if(b > t){
		printf("Bottom (%f) is greater than top (%f).\n", t, b);
		return 0;
	}
	
	std::ofstream model_out_stream;
	model_out_stream.open("model.sdf");
	
	//Output header
	model_out_stream << "<?xml version=\"1.0\"?>\n<sdf version=\"1.5\">\n\t<model name=\"";
	model_out_stream << name << "\">\n\t\t<pose>0 0 0 0 0 0</pose>\n\t\t<static>true</static>\n";
	model_out_stream << "\t\t<link name=\"body\">\n";
	
	//Output white base
	model_out_stream << "\t\t<visual name=\"white_bkg\">\n";
	model_out_stream << "\t\t\t<pose>";
	model_out_stream << (l+r) / 2.0 << " " << (b + t) / 2.0 << " ";
	model_out_stream << "-0.001 0 0 0</pose>\n";
	model_out_stream << "\t\t\t<geometry>\n\t\t\t\t<box>\n\t\t\t\t\t<size>";
	model_out_stream << (r - l) + 2 * e <<  " " << (t - b) + 2 * e << " ";
	model_out_stream << "0.002</size>\n";
	model_out_stream << "\t\t\t\t</box>\n\t\t\t</geometry>\n\t\t\t<material>\n\t\t\t\t<script>\n";
	model_out_stream << "\t\t\t\t\t<uri>file://media/materials/scripts/gazebo.material</uri>\n";
	model_out_stream << "\t\t\t\t\t<name>Gazebo/White</name>\n";
	model_out_stream << "\t\t\t\t</script>\n\t\t\t</material>\n\t\t</visual>\n\n\n";
	
	//Output targets
	bool even_row = true;
	int v = 0;
	for(double v_center = b - (s/2.0); v_center <= t; v_center += s * 2.0){
		
		double h_center;
		if(even_row){
			h_center = l - (s/2.0);
		} else {
			h_center = l + (s/2.0);
		}
		int h = 0;
		for( ; h_center <= r; h_center += s * 2.0){
			model_out_stream << "\t\t<visual name=\"sqr_" << h << "_" << v << "\">\n";
			model_out_stream << "\t\t\t<pose>" << h_center << " " << v_center << " 0 0 0 0</pose>\n";
			model_out_stream << "\t\t\t<geometry>\n\t\t\t\t<box>\n\t\t\t\t\t<size>";
			model_out_stream << s << " " << s << " 0.003 </size>\n";
			model_out_stream << "\t\t\t\t</box>\n\t\t\t</geometry>\n\t\t\t<material>\n\t\t\t\t<script>\n";
			model_out_stream << "\t\t\t\t\t<uri>file://media/materials/scripts/gazebo.material</uri>\n";
			model_out_stream << "\t\t\t\t\t<name>Gazebo/Black</name>\n";
			model_out_stream << "\t\t\t\t</script>\n\t\t\t</material>\n\t\t</visual>\n";
			
			h++;
		}
		
		v++;
		even_row = ! even_row;
	}
	
	//Output tailer
	model_out_stream << "\t\t</link>\n\t</model>\n</sdf>";
	
	model_out_stream.close();

	return 0;
}
