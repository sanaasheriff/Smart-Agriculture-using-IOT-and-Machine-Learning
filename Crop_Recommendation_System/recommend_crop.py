import tkinter as tk
from tkinter import ttk
import joblib

# Load the pre-trained model
model = joblib.load('crop_prediction_model.pkl')

# Initialize Tkinter window
root = tk.Tk()
root.title("Crop Prediction")
root.geometry("340x255") 

# Function to predict crop and update UI
def predict_crop():
    # Fetching data from Tkinter entry fields
    temp = float(temp_entry.get())
    humidity = float(humidity_entry.get())
    ph = float(ph_entry.get())
    soil_moisture = float(soil_moisture_entry.get())

    # Creating prediction data
    predict_data = [[temp, humidity, ph]]

    # Predicting crop
    predicted_crop = model.predict(predict_data)[0]

    # Display predicted crop
    result_label.config(text="Predicted Crop: " + predicted_crop)

style = ttk.Style()
style.configure('TLabel', background='#AEC6CF', font=('Arial', 15))
style.configure('TEntry', background='#AEC6CF', font=('Arial', 20))

# Add labels and entry fields for input parameters
temp_label = ttk.Label(root, text="Temperature (°C):")
temp_label.grid(row=0, column=0, padx=10, pady=5)
temp_entry = ttk.Entry(root)
temp_entry.grid(row=0, column=1, padx=10, pady=5)

humidity_label = ttk.Label(root, text="Humidity (%):")
humidity_label.grid(row=1, column=0, padx=10, pady=5)
humidity_entry = ttk.Entry(root)
humidity_entry.grid(row=1, column=1, padx=10, pady=5)

ph_label = ttk.Label(root, text="pH:")
ph_label.grid(row=2, column=0, padx=10, pady=5)
ph_entry = ttk.Entry(root)
ph_entry.grid(row=2, column=1, padx=10, pady=5)

soil_moisture_label = ttk.Label(root, text="Soil Moisture (%):")
soil_moisture_label.grid(row=4, column=0, padx=10, pady=5)
soil_moisture_entry = ttk.Entry(root)
soil_moisture_entry.grid(row=4, column=1, padx=10, pady=5)

# Add button to predict crop
predict_button = ttk.Button(root, text="Recommend Crop", width=15, command=predict_crop, style='Custom.TButton')
predict_button.grid(row=5, columnspan=2, padx=10, pady=10)

# Add label to display predicted crop
result_label = ttk.Label(root, text="", background='#AEC6CF', font=('Arial', 15))
result_label.grid(row=6, columnspan=2, padx=10, pady=5)

# Configure custom style for the button
style.configure('Custom.TButton', background='#AEC6CF', font=('Arial', 13), padding=6, width=12)

root.mainloop()
