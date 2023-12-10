#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofSetLineWidth(1);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	if (ofGetFrameNum() % 60 < 25) {

		this->noise_value += ofMap(ofGetFrameNum() % 60, 0, 25, 0.2, 0);
	}

	this->face.clear();
	this->frame.clear();

	float span = 0.5;
	float coordinate_param = 0.01;
	//float len = 170;
	//float z = len + span * 0.5;

	float angle;
	glm::highp_mat4 rotation;

	for (float len = 150; len > 130; len -= 2) { 

		float z = len + span * 0.5;

		float threshold_1 = 0.48;
		float threshold_2 = 0.52;
		for (int i = 0; i <= 6; i++) {

			for (float x = -len; x <= len; x += span) {

				for (float y = -len; y <= len; y += span) {

					if (i <= 4) {

						angle = PI * 0.5 * i;
						rotation = glm::rotate(glm::mat4(), angle, glm::vec3(0, 1, 0));
					}

					if (i == 5) {

						angle = PI * 0.5;
						rotation = glm::rotate(glm::mat4(), angle, glm::vec3(1, 0, 0));
					}

					if (i == 6) {

						angle = PI * 0.5 * 3;
						rotation = glm::rotate(glm::mat4(), angle, glm::vec3(1, 0, 0));
					}


					glm::vec3 noise_location = glm::vec4(x, y, z, 0) * rotation;
					auto noise_value = ofNoise(glm::vec4(noise_location * coordinate_param, this->noise_value));
					if (noise_value < threshold_1 || noise_value > threshold_2) { continue; }

					glm::vec3 loc_1 = glm::vec4(x - span, y, z, 0) * rotation;
					auto noise_1 = ofNoise(glm::vec4(loc_1 * coordinate_param, this->noise_value));
					glm::vec3 loc_2 = glm::vec4(x, y + span, z, 0) * rotation;
					auto noise_2 = ofNoise(glm::vec4(loc_2 * coordinate_param, this->noise_value));
					glm::vec3 loc_3 = glm::vec4(x + span, y, z, 0) * rotation;
					auto noise_3 = ofNoise(glm::vec4(loc_3 * coordinate_param, this->noise_value));
					glm::vec3 loc4 = glm::vec4(x, y - span, z, 0) * rotation;
					auto noise_4 = ofNoise(glm::vec4(loc4 * coordinate_param, this->noise_value));

					auto index = this->face.getNumVertices();
					vector<glm::vec3> vertices;

					vertices.push_back(glm::vec4(x - span * 0.5, y - span * 0.5, z, 0) * rotation);
					vertices.push_back(glm::vec4(x + span * 0.5, y - span * 0.5, z, 0) * rotation);
					vertices.push_back(glm::vec4(x + span * 0.5, y + span * 0.5, z, 0) * rotation);
					vertices.push_back(glm::vec4(x - span * 0.5, y + span * 0.5, z, 0) * rotation);

					this->face.addVertices(vertices);

					this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 2);
					this->face.addIndex(index + 0); this->face.addIndex(index + 2); this->face.addIndex(index + 3);

					ofColor face_color;
					face_color.setHsb(139, 180, ofMap(len, 150, 130, 255, 0));
					this->face.addColor(face_color);
					this->face.addColor(face_color); 
					this->face.addColor(face_color); 
					this->face.addColor(face_color);

					if (noise_1 < threshold_1 || noise_1 > threshold_2) {

						this->frame.addVertex(vertices[0]);
						this->frame.addVertex(vertices[3]);

						this->frame.addIndex(this->frame.getNumVertices() - 1);
						this->frame.addIndex(this->frame.getNumVertices() - 2);
					}

					if (noise_2 < threshold_1 || noise_2 > threshold_2) {

						this->frame.addVertex(vertices[2]);
						this->frame.addVertex(vertices[3]);

						this->frame.addIndex(this->frame.getNumVertices() - 1);
						this->frame.addIndex(this->frame.getNumVertices() - 2);
					}

					if (noise_3 < threshold_1 || noise_3 > threshold_2) {

						this->frame.addVertex(vertices[1]);
						this->frame.addVertex(vertices[2]);

						this->frame.addIndex(this->frame.getNumVertices() - 1);
						this->frame.addIndex(this->frame.getNumVertices() - 2);
					}

					if (noise_4 < threshold_1 || noise_4 > threshold_2) {

						this->frame.addVertex(vertices[0]);
						this->frame.addVertex(vertices[1]);

						this->frame.addIndex(this->frame.getNumVertices() - 1);
						this->frame.addIndex(this->frame.getNumVertices() - 2);
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 1.44);

	ofSetColor(39);
	this->frame.draw();

	this->face.draw();

	this->cam.end();

	/*
	int start = 1;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}