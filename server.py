from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/post-data', methods=['POST'])
def handle_post_data():
    if request.is_json:
        data = request.get_json()
        temp = data.get('temperature')
        humid = data.get('humidity')
        air_qual = data.get('airQuality')
        
        # membuat log data yang terkirim
        print(f"Received data - Temperature: {temp}, Humidity: {humid}, Air Quality: {air_qual}")
        
        # membuat response
        response = {
            "status": "success",
            "message": "Data received successfully",
            "data": data
        }
        return jsonify(response), 200
    else:
        return jsonify({"status": "error", "message": "Request must be JSON"}), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
