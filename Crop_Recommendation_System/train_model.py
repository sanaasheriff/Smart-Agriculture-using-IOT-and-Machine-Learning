import joblib
import pandas as pd
from sklearn.model_selection import train_test_split
import lightgbm as lgb

# Read the dataset
cropdf = pd.read_csv('cpdata.csv')

# Keep only the first 3 columns for training
X = cropdf.iloc[:, :3]
y = cropdf['label']

# Split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, shuffle=True, random_state=0)

# Build the LightGBM model
model = lgb.LGBMClassifier()
model.fit(X_train, y_train)

# Export the trained model
joblib.dump(model, 'crop_prediction_model.pkl')
