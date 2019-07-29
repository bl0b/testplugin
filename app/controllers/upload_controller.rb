class UploadController < ApplicationController
    skip_before_action :verify_authenticity_token, only: [:create]

    def create
        record = Upload.new
        record.uploaded_document = params[:payload]
        record.save!
        render plain: record.id.to_s
    end
end
